PYTHON ?= python3
.PHONY: help check test host-check verify prepare progress inventory disc-inventory match rebuild tools tools-check

help:
	@echo "tools: download and install the pinned compiler, wibo and MIPS binutils; tools-check: report them"
	@echo "verify/prepare: check your disc image and extract SLUS_205.15 into out/"
	@echo "check/test: ROM-free validation; host-check: compile the C on this machine and run its tests (WARNINGS=1 shows all compiler warnings)"
	@echo "match: compile every function and compare it with your executable; rebuild: the whole executable"
	@echo "progress: matched functions and bytes; inventory/disc-inventory: executable and disc metadata"

check:
	$(PYTHON) tools/project.py check

test: check
	$(PYTHON) -m unittest discover -s tests -v

host-check:
	cmake -S . -B build/host -DDOTR_HOST_WARNINGS=$(if $(WARNINGS),ON,OFF)
	cmake --build build/host
	ctest --test-dir build/host --output-on-failure

verify:
	$(PYTHON) tools/project.py verify

prepare:
	$(PYTHON) tools/project.py prepare

progress:
	$(PYTHON) tools/project.py progress

inventory:
	$(PYTHON) tools/research.py inventory

disc-inventory:
	$(PYTHON) tools/research.py disc

match:
	$(PYTHON) tools/match.py

rebuild: match
	$(PYTHON) tools/rebuild_exe.py

tools:
	$(PYTHON) tools/setup_tooling.py --download --install

tools-check:
	$(PYTHON) tools/runtime.py doctor
