# Graph - Explained

## What is a graph?

A graph is a collection of nodes (vertices) and edges connecting them. This implementation represents a directed (or general) graph where edges go from `src` to `dest` and have an associated `weight` (integer). Nodes store `uint64_t data`. The graph stores arrays of node pointers and edge pointers.

## Implementation details

- **Node**: `struct GraphNode { uint64_t data; struct GraphNode *next; }` (note: `next` field exists but isn't used in the adjacency storage model here; nodes are stored in a flat array)
- **Edge**: `struct GraphEdge { struct GraphNode *src; struct GraphNode *dest; int weight; }`
- **Graph**: `struct Graph { struct GraphNode **nodes; struct GraphEdge **edges; int num_nodes; int num_edges; }`
- `create_graph()` initializes empty arrays (NULL pointers) with counts 0
- `add_node()` allocates a node, stores data, appends to `nodes` array via `realloc`
- `add_edge()` checks `has_node()` for both endpoints; if valid, allocates edge and appends to `edges` array
- `remove_node()` finds and removes the node from `nodes` array (shifting), then removes all edges where `src == node` or `dest == node` (compacts edge array, skipping removed ones)
- `remove_edge()` finds and removes matching edge pointer
- `print_graph()` prints node data values space-separated
- `has_node()`/`has_edge()` do linear scans
- `path_exists()` does BFS from `src` to `dest`. It builds a simple queue (array-based) and a boolean `visited` array indexed by node positions in `nodes` array. Returns true immediately if reaches `dest`. Returns `false` on allocation failure.
- `connected()` checks if the graph is strongly connected in the sense of every node reachable from every other node (all-pairs reachability) using `path_exists()` - i.e. for every distinct pair (i,j), there is a path i->j.

## Operations (time complexity)

| Operation | What it does | Time Complexity |
|-----------|--------------|-----------------|
| `create_graph()` | Create empty graph | O(1) |
| `add_node()` | Add new node | O(n) due to `realloc`/copy of array |
| `add_edge()` | Add edge between existing nodes | O(n) due to `realloc` + O(n) for `has_node()` checks = O(n) |
| `remove_node()` | Remove node and incident edges | O(n + m) where n=num_nodes, m=num_edges |
| `remove_edge()` | Remove specific edge | O(m) |
| `print_graph()` | Print nodes | O(n) |
| `has_node()` | Check if node exists | O(n) |
| `has_edge()` | Check if edge exists | O(m) |
| `path_exists()` | BFS for path src->dest | O(V + E) for BFS on adjacency; V=num_nodes, E=num_edges |
| `connected()` | All-pairs reachability | O(V * (V + E)) |
| `free_graph()` | Free all memory | O(n + m) |

Note: the graph uses edge lists (array of edges) rather than adjacency lists - so traversing neighbors requires scanning all edges. That’s fine for small graphs but is less efficient than adjacency lists.

## Pros

- Simple representation (arrays of nodes/edges)
- Supports directed edges with weights
- Includes BFS path existence and connectivity check

## Cons

- Inefficient neighbor lookups (O(E) per node traversal)
- Linear scans for membership checks
- No dynamic indexing optimization
- Arrays grow by realloc (O(n) per add)
- `connected()` does all-pairs path_exists which is expensive

## When to use

- For learning graph concepts (small educational graphs)
- When you need explicit edge objects (with weights)
- For simple connectivity/path queries on small datasets

## Example

```c
#include "graph.h"

int main() {
    struct Graph *g = create_graph();
    add_node(g, 1);
    add_node(g, 2);
    struct GraphNode *n1 = g->nodes[0];
    struct GraphNode *n2 = g->nodes[1];
    add_edge(g, n1, n2, 5);
    bool p = path_exists(g, n1, n2); // true
    bool c = connected(g);           // depends on structure
    free_graph(g);
    return 0;
}
```
