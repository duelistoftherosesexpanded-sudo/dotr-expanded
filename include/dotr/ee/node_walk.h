#ifndef DOTR_EE_NODE_WALK_H
#define DOTR_EE_NODE_WALK_H

/* 0x1efbd0: a recursive walk over records linked by 28-bit offsets from a base.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/models.md. */

/* A record of the walk at 0x1efbd0: only the two link words are used. Each is
 * zero or an offset from the walk's base in its low 28 bits. */
struct DotrWalkNode {
    int unknown_00[7];
    unsigned int child;    /* 0x1c */
    int unknown_20[3];
    unsigned int next;     /* 0x2c */
};

/* lSzModel_TraceGNode: pointer types are assumed from the dereferences; object_table.h
 * declares the same address with int arguments for its callers. */
struct DotrWalkNode *lSzModel_TraceGNode(int targetNodeId, char *base, struct DotrWalkNode *node, int *nodeCounter, int *depth,
                                   int *maxDepth, void (*visit)(int, struct DotrWalkNode *));

#endif
