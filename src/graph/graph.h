/* Draconis DSA Graph Library */

struct Node {
    int data;
    struct Node* next;
};

struct Edge {
    struct Node* src;
    struct Node* dest;
    int weight;
};

struct Graph {
    struct Node** nodes;
    struct Edge** edges;
    int num_nodes;
    int num_edges;
};

struct Graph* create_graph(int num_nodes, int num_edges);

void add_node(struct Graph* graph, int data);
void add_edge(struct Graph* graph, struct Node* src, struct Node* dest, int weight);

void remove_node(struct Graph* graph, struct Node* node);
void remove_edge(struct Graph* graph, struct Edge* edge);

void print_graph(struct Graph* graph);
void free_graph(struct Graph* graph);

bool has_node(struct Graph* graph, struct Node* node);
bool has_edge(struct Graph* graph, struct Node* src, struct Node* dest);
bool path_exists(struct Graph* graph, struct Node* src, struct Node* dest);
