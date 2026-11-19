/* Synthetic host tests for the node_walk group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/node_walk.h"

/* Tree, in visit order: 0 { 1 { 2 }, 3 }, 4.  nodes[0] is unused so that
 * no link offset is zero for a linked node. */
static struct DotrWalkNode nodes[6];
static int seen[8], seen_nodes[8], seen_count;

static void visit(int count, struct DotrWalkNode *node)
{
    assert(seen_count < 8);
    seen[seen_count] = count;
    seen_nodes[seen_count++] = (int)(node - nodes);
}

static unsigned int link(int index)
{
    /* the high four bits are discarded by the walk */
    return 0xa0000000u | (unsigned int)(index * sizeof(nodes[0]));
}

static void build(void)
{
    assert(offsetof(struct DotrWalkNode, child) == 0x1c && offsetof(struct DotrWalkNode, next) == 0x2c);
    memset(nodes, 0, sizeof(nodes));
    /* node k of the tree is stored at nodes[k + 1] */
    nodes[1].child = link(2);
    nodes[1].next = link(5);
    nodes[2].child = link(3);
    nodes[2].next = link(4);
    seen_count = 0;
}

/* lSzModel_TraceGNode branch coverage: one assertion per branch outcome (each null
 * pointer argument, target found at the start, through a child, through a
 * next link, not found, negative target) and per side of the max comparison. */
int main(void)
{
    char *base = (char *)nodes;
    int count, depth, max, i;

    /* full walk with every pointer: not found, all five visited in order */
    build();
    count = 0;
    depth = 1;
    max = 0;
    assert(lSzModel_TraceGNode(-1, base, &nodes[1], &count, &depth, &max, visit) == 0);
    assert(count == 5 && depth == 1 && max == 3 && seen_count == 5);
    for (i = 0; i < 5; i++) {
        assert(seen[i] == i && seen_nodes[i] == i + 1);
    }

    /* max already equal or larger is left alone */
    build();
    count = 0;
    depth = 3;
    max = 3;
    assert(lSzModel_TraceGNode(0, base, &nodes[1], &count, &depth, &max, visit) == &nodes[1]);
    assert(count == 0 && depth == 3 && max == 3 && seen_count == 1);
    max = 2;
    assert(lSzModel_TraceGNode(0, base, &nodes[1], &count, &depth, &max, 0) == &nodes[1] && max == 3);

    /* found two children down: depth is raised once at each level that has one */
    build();
    count = 0;
    depth = 1;
    max = 0;
    assert(lSzModel_TraceGNode(2, base, &nodes[1], &count, &depth, &max, visit) == &nodes[3]);
    assert(count == 2 && depth == 2 && max == 3 && seen_count == 3);

    /* found through a child's next link, and through the root's next link */
    build();
    count = 0;
    depth = 1;
    max = 0;
    assert(lSzModel_TraceGNode(3, base, &nodes[1], &count, &depth, &max, visit) == &nodes[4]);
    assert(count == 3 && depth == 2 && seen_count == 4);
    build();
    count = 0;
    depth = 1;
    assert(lSzModel_TraceGNode(4, base, &nodes[1], &count, &depth, &max, visit) == &nodes[5]);
    assert(count == 4 && depth == 1 && seen_count == 5);

    /* target past the end: not found */
    build();
    count = 0;
    depth = 1;
    assert(lSzModel_TraceGNode(5, base, &nodes[1], &count, &depth, &max, visit) == 0 && count == 5 && depth == 1);

    /* no depth pointer: children start from 1 + 1, nothing is written back */
    build();
    count = 0;
    max = 0;
    assert(lSzModel_TraceGNode(2, base, &nodes[1], &count, 0, &max, 0) == &nodes[3]);
    assert(count == 2 && max == 3 && seen_count == 0);

    /* no max pointer */
    build();
    count = 0;
    depth = 1;
    assert(lSzModel_TraceGNode(2, base, &nodes[1], &count, &depth, 0, visit) == &nodes[3] && depth == 2);
    return 0;
}
