#include "dotr/ee/node_walk.h"

struct DotrWalkNode *lSzModel_TraceGNode(int targetNodeId, char *base, struct DotrWalkNode *node, int *nodeCounter, int *depth,
                                   int *maxDepth, void (*visit)(int, struct DotrWalkNode *))
{
    struct DotrWalkNode *found;
    int d;

    if (maxDepth != 0 && depth != 0 && *maxDepth < *depth) {
        *maxDepth = *depth;
    }
    for (;;) {
        if (visit != 0) {
            visit(*nodeCounter, node);
        }
        if (targetNodeId >= 0 && *nodeCounter == targetNodeId) {
            return node;
        }
        (*nodeCounter)++;
        if (node->child != 0) {
            d = (depth != 0 ? *depth : 1) + 1;
            found = lSzModel_TraceGNode(targetNodeId, base, (struct DotrWalkNode *)(base + (node->child & 0xfffffff)), nodeCounter, &d,
                                  maxDepth, visit);
            if (found != 0) {
                if (depth != 0) {
                    (*depth)++;
                }
                return found;
            }
        }
        if (node->next == 0) {
            break;
        }
        node = (struct DotrWalkNode *)(base + (node->next & 0xfffffff));
    }
    return 0;
}
