#include <iostream>
#include <stack>
#include "Profiler.h"

enum {
    COLOR_WHITE = 0,
    COLOR_GRAY,
    COLOR_BLACK
};

typedef struct _Node {
    struct _Node **adj;
    int key;
    int adjSize;
    int color;
    int dist;
    int d, f;
    int low;
    _Node *parent, *next;
} Node;

typedef struct {
    int nrNodes;
    Node **v;
} Graph;

typedef struct _List {
    Node *node;
    _List *prev, *next;
} List;

void insert_first(List **first, Node *node) {
    List *element = new List(node, nullptr, nullptr);
    if ((*first) == nullptr) {
        *first = element;
    } else {
        element->next = *first;
        (*first)->prev = element;
        *first = element;
    }
}

void insert_first(List **first, List *list) {
    if ((*first) == nullptr) {
        *first = list;
    } else {
        List *last = list;
        while (last->next) {
            last = last->next;
        }
        last->next = *first;
        (*first)->prev = last;
        *first = list;
    }
}

void print_list(List *list) {
    while (list) {
        std::cout << list->node->key << " ";
        list = list->next;
    }
}

int t;
bool cycle;

void DFS_VISIT(Graph *g, Node* u, Operation &op) {
    u->d = ++t;
    u->color = COLOR_GRAY;
    op.count(2);
    for (int y = 0; y < u->adjSize; y++) {
        op.count();
        if (u->adj[y]->color == COLOR_WHITE) {
            op.count();
            u->adj[y]->parent = u;
            DFS_VISIT(g, u->adj[y], op);
        }
    }
    u->color = COLOR_BLACK;
    u->f = ++t;
    op.count(2);
}

void DFS(Graph *g, Operation &op) {
    for (int u = 0; u < g->nrNodes; u++)
        g->v[u]->color = COLOR_WHITE, g->v[u]->parent = nullptr, op.count(2);
    t = 0;
    for (int u = 0; u < g->nrNodes; u++) {
        op.count();
        if (g->v[u]->color == COLOR_WHITE)
            DFS_VISIT(g, g->v[u], op);
    }
}

void DFS_VISIT_PRINT(Graph *g, int u, int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << g->v[u]->key << std::endl;

    g->v[u]->d = ++t;
    g->v[u]->color = COLOR_GRAY;

    for (int y = 0; y < g->v[u]->adjSize; y++) {
        if (g->v[u]->adj[y]->color == COLOR_WHITE) {
            g->v[u]->adj[y]->parent = g->v[u];
            DFS_VISIT_PRINT(g, g->v[u]->adj[y]->key, depth + 1);
        }
    }

    g->v[u]->color = COLOR_BLACK;
    g->v[u]->f = ++t;
}

void DFS_PRINT(Graph *g) {
    std::cout << std::endl << "DFS Tree:" << std::endl;
    for (int u = 0; u < g->nrNodes; u++)
        g->v[u]->color = COLOR_WHITE, g->v[u]->parent = nullptr;
    for (int u = 0; u < g->nrNodes; u++) {
        if (g->v[u]->color == COLOR_WHITE) {
            DFS_VISIT_PRINT(g, u, 0);
        }
    }
}

void DFS_VISIT_SORT(Graph *g, int u, List **first) {
    g->v[u]->d = ++t;
    g->v[u]->color = COLOR_GRAY;
    for (int y = 0; y < g->v[u]->adjSize; y++)
        if (g->v[y]->color == COLOR_WHITE) {
            g->v[y]->parent = g->v[u];
            DFS_VISIT_SORT(g, y, first);
        }
         else if(g->v[y]->color == COLOR_GRAY)
            cycle = true;
    g->v[u]->color = COLOR_BLACK;
    g->v[u]->f = ++t;
    insert_first(first, g->v[u]);
}

void DFS_SORT(Graph *g, List **first) {
    for (int u = 0; u < g->nrNodes; u++)
        g->v[u]->color = COLOR_WHITE, g->v[u]->parent = nullptr;
    t = 0;
    for (int u = 0; u < g->nrNodes; u++) {
        if (g->v[u]->color == COLOR_WHITE)
            DFS_VISIT_SORT(g, u, first);
    }
}

List *topological_sort(Graph *g) {
    List *list = nullptr;
    DFS_SORT(g, &list);
    return list;
}

void DFS_VISIT_TARJAN(Graph *g, int u, std::stack<Node *> &sccStack) {
    g->v[u]->d = ++t;
    g->v[u]->low = g->v[u]->d;
    g->v[u]->color = COLOR_GRAY;
    sccStack.push(g->v[u]);

    for (int y = 0; y < g->v[u]->adjSize; y++) {
        if (g->v[u]->adj[y]->color == COLOR_WHITE) {
            g->v[u]->adj[y]->parent = g->v[u];
            DFS_VISIT_TARJAN(g, g->v[u]->adj[y]->key, sccStack);
            g->v[u]->low = std::min(g->v[u]->low, g->v[u]->adj[y]->low);
        } else if (g->v[u]->adj[y]->color == COLOR_GRAY) {
            g->v[u]->low = std::min(g->v[u]->low, g->v[u]->adj[y]->d);
        }
    }


    if (g->v[u]->low == g->v[u]->d) {
        List *sccList = nullptr;
        Node *v;
        do {
            v = sccStack.top();
            sccStack.pop();
            insert_first(&sccList, v);
        } while (v != g->v[u]);

        std::cout << "SCC: ";
        print_list(sccList);
        std::cout << std::endl;
    }
}

void tarjan_SCC(Graph *g) {
    std::stack<Node *> sccStack;
    for (int u = 0; u < g->nrNodes; u++)
        g->v[u]->color = COLOR_WHITE, g->v[u]->parent = nullptr;
    t = 0;
    for (int u = 0; u < g->nrNodes; u++)
        if (g->v[u]->color == COLOR_WHITE)
            DFS_VISIT_TARJAN(g, u, sccStack);
}

void allocate_graph(Graph *g){
    g->nrNodes = 5;
    g->v = (Node **) malloc(g->nrNodes * sizeof(Node *));

    for (int i = 0; i < g->nrNodes; i++) {
        g->v[i] = (Node *) malloc(sizeof(Node));
    }

    g->v[0]->adjSize = 2, g->v[0]->key = 0;
    g->v[1]->adjSize = 1, g->v[1]->key = 1;
    g->v[2]->adjSize = 1, g->v[2]->key = 2;
    g->v[3]->adjSize = 3, g->v[3]->key = 3;
    g->v[4]->adjSize = 0, g->v[4]->key = 4;

    for (int i = 0; i < g->nrNodes; i++) {
        g->v[i]->adj = (Node **) malloc(g->v[i]->adjSize * sizeof(Node *));
    }

    g->v[0]->adj[0] = g->v[1];
    g->v[0]->adj[1] = g->v[3];
    //g->v[0]->adj[2] = g->v[4];

    g->v[1]->adj[0] = g->v[2];
    //g->v[1]->adj[1] = g->v[4];

    g->v[2]->adj[0] = g->v[0];

    g->v[3]->adj[0] = g->v[1];
    g->v[3]->adj[1] = g->v[0];
    g->v[3]->adj[2] = g->v[2];

    //g->v[4]->adj[0] = g->v[1];
    //g->v[4]->adj[1] = g->v[3];
}

void demo() {
    Profiler profiler("DFS");
    Operation op = profiler.createOperation("Variable edges", 1);
    Graph g,g1,g2;
    allocate_graph(&g);
    allocate_graph(&g1);
    allocate_graph(&g2);

    std::cout<<"The edges of the graph are: ";
    for(int i = 0; i < g2.nrNodes; i++){
        for(int j = 0; j < g2.v[i]->adjSize; j++)
            std::cout<<"("<<i<<", "<<g2.v[i]->adj[j]->key<<")"<<", ";
    }
    DFS_PRINT(&g);
    std::cout << std::endl;
    for (int i = 0; i < g.nrNodes; i++)
        std::cout << "Node " << i << " times: " << g.v[i]->d << " " << g.v[i]->f << std::endl;
    t = 0;
    std::cout << std::endl << "Result of topological sort: ";
    cycle = false;
    List *list = topological_sort(&g1);
    if(cycle)
        std::cout<<"The graph is not acyclic";
    else print_list(list);
    std::cout<<std::endl<<std::endl<<"Now for an acyclic graph: ";
    g2.v[0]->adjSize=1;
    g2.v[1]->adjSize=2;
    g2.v[2]->adjSize=1;
    g2.v[3]->adjSize=1;
    g2.v[4]->adjSize=0;

    g2.v[0]->adj[0] = g.v[1];
    g2.v[1]->adj[0] = g.v[2];
    g2.v[1]->adj[1] = g.v[4];
    g2.v[2]->adj[0] = g.v[3];
    g2.v[3]->adj[0] = g.v[4];

    std::cout<<std::endl<<"The edges of the new graph are: ";
    for(int i = 0; i < g2.nrNodes; i++){
        for(int j = 0; j < g2.v[i]->adjSize; j++)
            std::cout<<"("<<i<<", "<<g2.v[i]->adj[j]->key<<")"<<", ";
    }
    std::cout << std::endl;

    std::cout<<std::endl<<std::endl;
    list = topological_sort(&g2);
    std::cout << "Result of topological sort: ";
    print_list(list);
    std::cout<<std::endl;
    for (int i = 0; i < g2.nrNodes; i++)
        std::cout << "Node " << i << " times: " << g2.v[i]->d << " " << g2.v[i]->f << std::endl;


    std::cout << std::endl << "Strongly Connected Components for the cyclic graph:"<<std::endl;
    tarjan_SCC(&g);
    std::cout << std::endl << "Strongly Connected Components for the acyclic graph:"<<std::endl;
    tarjan_SCC(&g2);
}

bool isEdge(Graph *g, int x, int y) {
    for (int i = 0; i < g->v[x]->adjSize; i++)
        if (g->v[i]->adj[i] == g->v[y])
            return true;
    return false;
}

void performance() {
    Profiler profiler("DFS");
    srand(time(nullptr));
    int v, e;
    v = 100;
    for (e = 1000; e <= 4500; e += 100) {
        Operation op = profiler.createOperation("Variable edges", e);
        Graph g;
        g.nrNodes = v;
        g.v = (Node **) malloc(g.nrNodes * sizeof(Node *));
        for (int i = 0; i < g.nrNodes; i++) {
            g.v[i] = (Node *) malloc(sizeof(Node));
            g.v[i]->adjSize = 0;
        }
        for (int i = 0; i < g.nrNodes; i++) {
            g.v[i]->adj = (Node **) malloc(g.nrNodes * sizeof(Node *));
        }
        for (int i = 0; i < e; i++) {
            int x = rand() % g.nrNodes;
            int y = rand() % g.nrNodes;
            if (x != y && !isEdge(&g, x, y)) {
                g.v[x]->adj[g.v[x]->adjSize++] = g.v[y];
            }
        }
        DFS(&g, op);
    }
    e = 4500;
    for (v = 100; v <= 200; v += 10) {
        Operation op1 = profiler.createOperation("Variable vertices", v);
        Graph g;
        g.nrNodes = v;
        g.v = (Node **) malloc(g.nrNodes * sizeof(Node *));
        for (int i = 0; i < g.nrNodes; i++) {
            g.v[i] = (Node *) malloc(sizeof(Node));
            g.v[i]->adjSize = 0;
        }
        for (int i = 0; i < g.nrNodes; i++) {
            g.v[i]->adj = (Node **) malloc(g.nrNodes * sizeof(Node *));
        }
        for (int i = 0; i < e; i++) {
            int x = rand() % g.nrNodes;
            int y = rand() % g.nrNodes;
            if (x != y && !isEdge(&g, x, y)) {
                g.v[x]->adj[g.v[x]->adjSize++] = g.v[y];
            }
        }
        DFS(&g, op1);
    }
    profiler.showReport();
}

int main() {
    demo();
    //performance();
    return 0;
}
