#include <stdlib.h>
#include <string.h>
#include "bfs.h"
#include <climits>
#include <queue>
#include <iostream>

//int get_neighbors(const Grid *grid, Point p, Point neighb[]) {
//    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
//    // the point p will have at most 4 neighbors (up, down, left, right)
//    // avoid the neighbors that are outside the grid limits or fall into a wall
//    // note: the size of the array neighb is guaranteed to be at least 4
//    int nr_neighb = 0;
//    if (p.row - 1 >= 0 && grid->mat[p.row - 1][p.col] != 1)
//        neighb[nr_neighb].row = p.row - 1, neighb[nr_neighb].col = p.col, nr_neighb++;
//    if (p.row + 1 < grid->rows && grid->mat[p.row + 1][p.col] != 1)
//        neighb[nr_neighb].row = p.row + 1, neighb[nr_neighb].col = p.col, nr_neighb++;
//    if (p.col - 1 >= 0 && grid->mat[p.row][p.col - 1] != 1)
//        neighb[nr_neighb].row = p.row, neighb[nr_neighb].col = p.col - 1, nr_neighb++;
//    if (p.col + 1 < grid->cols && grid->mat[p.row][p.col + 1] != 1)
//        neighb[nr_neighb].row = p.row, neighb[nr_neighb].col = p.col + 1, nr_neighb++;
//    return nr_neighb;
//}

int get_neighbors(const Grid *grid, Point p, Point neighb[]) {
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4
    int nr_neighb = 0;
    if (p.row - 2 >= 0 && p.col - 1 >= 0)
        neighb[nr_neighb].row = p.row - 2, neighb[nr_neighb].col = p.col - 1, nr_neighb++;
    if (p.row - 2 >= 0 && p.col + 1 < grid->cols)
        neighb[nr_neighb].row = p.row - 2, neighb[nr_neighb].col = p.col + 1, nr_neighb++;
    if (p.row - 1 >= 0 && p.col - 2 >= 0)
        neighb[nr_neighb].row = p.row - 1, neighb[nr_neighb].col = p.col - 2, nr_neighb++;
    if (p.row - 1 >= 0 && p.col + 2 < grid->cols)
        neighb[nr_neighb].row = p.row - 1, neighb[nr_neighb].col = p.col + 2, nr_neighb++;
    if (p.row + 2 < grid->rows && p.col - 1 >= 0)
        neighb[nr_neighb].row = p.row + 2, neighb[nr_neighb].col = p.col - 1, nr_neighb++;
    if (p.row + 2 < grid->rows && p.col + 1 < grid->cols)
        neighb[nr_neighb].row = p.row + 2, neighb[nr_neighb].col = p.col + 1, nr_neighb++;
    if (p.row + 1 < grid->rows && p.col - 2 >= 0)
        neighb[nr_neighb].row = p.row + 1, neighb[nr_neighb].col = p.col - 2, nr_neighb++;
    if (p.row + 1 < grid->rows && p.col + 2 < grid->cols)
        neighb[nr_neighb].row = p.row + 1, neighb[nr_neighb].col = p.col + 2, nr_neighb++;
    return nr_neighb;
}

void grid_to_graph(const Grid *grid, Graph *graph) {
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[8];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (grid->mat[i][j] == 0) {
                nodes[i][j] = (Node *) malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            } else {
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node **) malloc(graph->nrNodes * sizeof(Node *));
    k = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (nodes[i][j] != NULL) {
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for (i = 0; i < graph->nrNodes; ++i) {
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if (graph->v[i]->adjSize != 0) {
            graph->v[i]->adj = (Node **) malloc(graph->v[i]->adjSize * sizeof(Node *));
            k = 0;
            for (j = 0; j < graph->v[i]->adjSize; ++j) {
                if (neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0) {
                    graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if (k < graph->v[i]->adjSize) {
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node **) realloc(graph->v[i]->adj, k * sizeof(Node *));
            }
        }
    }
}

void free_graph(Graph *graph) {
    if (graph->v != NULL) {
        for (int i = 0; i < graph->nrNodes; ++i) {
            if (graph->v[i] != NULL) {
                if (graph->v[i]->adj != NULL) {
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void enqueue(Queue **q, Node *x) {
    (*q)->queue[(*q)->tail] = x;
    if ((*q)->tail == (*q)->length - 1)
        (*q)->tail = 0;
    else (*q)->tail++;
}

Node *dequeue(Queue **q) {
    Node *x = (*q)->queue[(*q)->head];
    if ((*q)->head == (*q)->length - 1)
        (*q)->head = 0;
    else (*q)->head++;
    return x;
}

void bfs(Graph *graph, Node *s, Operation *op) {
    // TODO: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();

    for (int u = 0; u < graph->nrNodes; u++) {
        if (op) op->count(3);
        graph->v[u]->color = COLOR_WHITE;
        graph->v[u]->dist = INT_MAX;
        graph->v[u]->parent = nullptr;
    }
    if (op) op->count(3);
    s->color = COLOR_GRAY;
    s->dist = 0;
    std::queue<Node *> q;
    q.push(s);

    if (op) op->count();
    while (!q.empty()) {
        if (op) op->count(2);
        Node *u = q.front();
        q.pop();
        for (int v = 0; v < u->adjSize; v++) {
            if (op) op->count();
            if (u->adj[v]->color == COLOR_WHITE) {
                if (op) op->count(4);
                u->adj[v]->color = COLOR_GRAY;
                u->adj[v]->dist = u->dist + 1;
                u->adj[v]->parent = u;
                q.push(u->adj[v]);
            }
        }
        if (op) op->count();
        u->color = COLOR_BLACK;
    }
}

void pretty_print(int p[], Point repr[], int n, int x, int depth) {
    for (int j = 0; j < depth; j++) {
        std::cout << "       ";
    }
    std::cout << "(" << repr[x].row << ", " << repr[x].col << ")" << std::endl;
    for (int i = 0; i < n; i++) {
        if (p[i] == x) {
            pretty_print(p, repr, n, i, depth + 1);
        }
    }
}

void print_bfs_tree(Graph *graph) {
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int *) malloc(graph->nrNodes * sizeof(int));
    for (int i = 0; i < graph->nrNodes; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            transf[i] = n;
            ++n;
        } else {
            transf[i] = -1;
        }
    }
    if (n == 0) {
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int *) malloc(n * sizeof(int));
    repr = (Point *) malloc(n * sizeof(Node));
    for (int i = 0; i < graph->nrNodes && !err; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            if (transf[i] < 0 || transf[i] >= n) {
                err = 1;
            } else {
                repr[transf[i]] = graph->v[i]->position;
                if (graph->v[i]->parent == NULL) {
                    p[transf[i]] = -1;
                } else {
                    err = 1;
                    for (int j = 0; j < graph->nrNodes; ++j) {
                        if (graph->v[i]->parent == graph->v[j]) {
                            if (transf[j] >= 0 && transf[j] < n) {
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if (!err) {
        // TODO: pretty print the BFS tree
        // the parent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        int start = 0;
        for (int i = 0; i < n; i++)
            if (p[i] == -1) {
                start = i;
                break;
            }
        pretty_print(p, repr, n, start, 0);

    }

    if (p != NULL) {
        free(p);
        p = NULL;
    }
    if (repr != NULL) {
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[]) {
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    bfs(graph, start);
    int i = 0;
    path[i++] = end;
    Node *x = end;
    while (x != start && i <= graph->nrNodes) {
        x = x->parent;
        path[i++] = x;
    }
    if (x == start) {
        path[i++] = x;
        for (int j = 0; j < i / 2; j++)
            std::swap(path[j], path[i - j - 1]);
        return i;
    }

    return -1;
}

bool isEdge(Node *node1, Node *node2) {
    for (int i = 0; i < node1->adjSize; ++i) {
        if (node1->adj[i] == node2) {
            return true;
        }
    }
    return false;
}

void performance() {
    int n, i;
    Profiler p("bfs");
    srand(time(nullptr));

    // vary the number of edges
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node **) malloc(graph.nrNodes * sizeof(Node *));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node *) malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected

        int i;
        for (i = 0; i < 99; i++) {
            graph.v[i]->adj = (Node **) malloc(100 * sizeof(Node *));
            graph.v[i]->adjSize = 0;
            graph.v[i]->adj[graph.v[i]->adjSize++] = graph.v[i + 1];
            graph.v[i + 1]->parent = graph.v[i];
        }
        graph.v[99]->adj = (Node **) malloc(100 * sizeof(Node *));
        graph.v[99]->adjSize = 0;
        while (i < n) {
            int x = rand() % graph.nrNodes;
            int y = rand() % graph.nrNodes;
            if (x != y && !isEdge(graph.v[x], graph.v[y])) {
                graph.v[x]->adj[graph.v[x]->adjSize++] = graph.v[y];
                graph.v[y]->parent = graph.v[x];
                i++;
            }
        }

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node **) malloc(graph.nrNodes * sizeof(Node *));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node *) malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected

        int i;
        for (i = 0; i < n - 1; i++) {
            graph.v[i]->adj = (Node **) malloc(100 * sizeof(Node *));
            graph.v[i]->adjSize = 0;
            graph.v[i]->adj[graph.v[i]->adjSize++] = graph.v[i + 1];
            graph.v[i + 1]->parent = graph.v[i];
        }
        graph.v[n - 1]->adj = (Node **) malloc(100 * sizeof(Node *));
        graph.v[n - 1]->adjSize = 0;
        while (i < 4500) {
            int x = rand() % graph.nrNodes;
            int y = rand() % graph.nrNodes;
            if (!isEdge(graph.v[x], graph.v[y])) {
                graph.v[x]->adj[graph.v[x]->adjSize++] = graph.v[y];
                graph.v[y]->parent = graph.v[x];
                i++;
            }
        }

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
