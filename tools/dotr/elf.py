"""ELF32 inventory and checked address conversion, standard library only."""

import hashlib
import struct
from .binary import elf_metadata

SECTION_TYPES = {0: "NULL", 1: "PROGBITS", 2: "SYMTAB", 3: "STRTAB", 4: "RELA",
                 8: "NOBITS", 9: "REL", 11: "DYNSYM", 0x70000006: "MIPS_REGINFO"}


def inventory(data, *, allowed_types=(2,)):
    result = elf_metadata(data, allowed_types=allowed_types)
    header = struct.unpack_from("<HHIIIIIHHHHHH", data, 16)
    shoff, shsize, count, names_index = header[5], header[10], header[11], header[12]
    if not count or shsize != 40 or names_index >= count or shoff + shsize * count > len(data):
        raise ValueError("unsupported or invalid ELF section table")
    fields = ("name_offset", "type", "flags", "address", "offset", "size", "link", "info", "alignment", "entry_size")
    sections = [dict(zip(fields, struct.unpack_from("<10I", data, shoff + i * shsize))) for i in range(count)]
    names_section = sections[names_index]
    if names_section["type"] != 3 or names_section["offset"] + names_section["size"] > len(data):
        raise ValueError("invalid ELF section-name string table")
    names = data[names_section["offset"]:names_section["offset"] + names_section["size"]]
    for i, section in enumerate(sections):
        offset, size = section["offset"], section["size"]
        if section["type"] not in {0, 8} and offset + size > len(data):
            raise ValueError("ELF section exceeds file")
        name_offset = section.pop("name_offset")
        if name_offset >= len(names) or b"\0" not in names[name_offset:]:
            raise ValueError("invalid ELF section name")
        section["name"] = names[name_offset:names.index(b"\0", name_offset)].decode("ascii")
        section["index"] = i
        section["type_name"] = SECTION_TYPES.get(section["type"], hex(section["type"]))
    result.update(sha256=hashlib.sha256(data).hexdigest(), size=len(data), sections=sections,
                  section_table_offset=shoff, section_name_table_index=names_index)
    result["comments"] = [data[s["offset"]:s["offset"] + s["size"]].rstrip(b"\0").decode("ascii").split("\0")
                          for s in sections if s["name"] == ".comment"]
    return result


def symbol_table(data, metadata):
    """Read ordinary ELF32 symbols, including compiler-produced function sizes."""
    symbols = []
    for section in metadata["sections"]:
        if section["type"] != 2:
            continue
        if section["entry_size"] != 16 or section["size"] % 16 or section["link"] >= len(metadata["sections"]):
            raise ValueError("invalid ELF symbol table")
        names_section = metadata["sections"][section["link"]]
        if names_section["type"] != 3:
            raise ValueError("invalid ELF symbol string table")
        names = data[names_section["offset"]:names_section["offset"] + names_section["size"]]
        for offset in range(section["offset"], section["offset"] + section["size"], 16):
            name, value, size, info, other, index = struct.unpack_from("<IIIBBH", data, offset)
            if name >= len(names) or b"\0" not in names[name:]:
                raise ValueError("invalid ELF symbol name")
            symbols.append(dict(name=names[name:names.index(b"\0", name)].decode("ascii"),
                                value=value, size=size, type=info & 15, binding=info >> 4,
                                section_index=index, other=other))
    return symbols


def file_offset(metadata, address, size=1):
    if size < 1 or address < 0 or address + size > 0x100000000:
        raise ValueError("invalid address range")
    matches = [s["offset"] + address - s["vaddr"] for s in metadata["program_headers"]
               if s["type"] == 1 and s["vaddr"] <= address and address + size <= s["vaddr"] + s["filesz"]]
    if len(matches) != 1:
        raise ValueError("address range is not in one unique file-backed load segment")
    return matches[0]


def rename_function_sections(data, names):
    """Give each named function's section a unique `.text.<name>` name in a relocatable object.

    Only section-name offsets and an appended name table change; code, symbols and
    relocations are untouched. This lets a linker script place functions individually.
    """
    metadata = inventory(data, allowed_types=(1,))
    functions = {s["name"]: s for s in symbol_table(data, metadata) if s["type"] == 2 and s["section_index"]}
    table = metadata["sections"][metadata["section_name_table_index"]]
    strings = bytearray(data[table["offset"]:table["offset"] + table["size"]])
    result = bytearray(data)
    seen = set()
    for name in names:
        if name not in functions:
            raise ValueError(f"object does not define function {name}")
        index = functions[name]["section_index"]
        if index in seen or index >= len(metadata["sections"]) or functions[name]["value"] != 0:
            raise ValueError(f"function {name} does not start its own section")
        seen.add(index)
        struct.pack_into("<I", result, metadata["section_table_offset"] + 40 * index, len(strings))
        strings += f".text.{name}".encode("ascii") + b"\0"
    header = metadata["section_table_offset"] + 40 * metadata["section_name_table_index"]
    struct.pack_into("<II", result, header + 16, len(result), len(strings))
    return bytes(result + strings)
