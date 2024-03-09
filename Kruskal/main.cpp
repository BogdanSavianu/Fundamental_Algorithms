/*  Am folosit operatiile pe multimi disjuncte pentru a implementa algoritmul lui Kruskal de a afla arborele minim de acoperire.
Pentru asta sortam muchiile in ordinea ponderilor si lexicografic. Apoi luam muchiile pe rand si, daca capetele muchiei sunt din multimi
diferite (adica nu formeaza ciclu), o adaugam in arbore si facem asta pana avem n-1 muchii, unde n este numarul de noduri.
    Make-Set are complexitatea O(1), Link tot O(1), Find-Set si Union O(log V).*/


#include <bits/stdc++.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 100
using namespace std;

Profiler profiler("Kruskal");

typedef struct node {
    node *parent = nullptr;
    int rank;
} Set;

typedef struct edge {
    int u, v, weight;
} Edge;

void MakeSet(Set **x, int n) {
    Operation op = profiler.createOperation("opMake", n);
    op.count(2);
    (*x)->parent = *x;
    (*x)->rank = 0;
}

void Link(Set *x, Set *y, int n) {
    Operation op = profiler.createOperation("opUnion", n);
    op.count();
    if (x->rank > y->rank) {
        op.count();
        y->parent = x;
    } else {
        op.count(2);
        x->parent = y;
        if (x->rank == y->rank)
            y->rank++, op.count();
    }
}

Set *Find_Set(Set *x, int n, Operation& op) {
    op.count();
    if (x != x->parent)
        x->parent = Find_Set(x->parent, n), op.count();
    return x->parent;
}

void Union(Set *x, Set *y, int n) {
    Operation op = profiler.createOperation("opUnion", n);

    Link(Find_Set(x, n,op), Find_Set(y, n,op), n);
}

int compare(Edge a, Edge b) {
    if (a.weight == b.weight) {
        if (a.u == b.u) {
            if (a.v < b.v)
                return -1;
            else return 1;
        }
        else if(a.u < b.u)
            return -1;
        else return 1;
    }
    if(a.weight < b.weight)
        return -1;
    return 1;
}

void insertionSort(vector<Edge> &edges, int l, int r) {
    for (int i = l + 1; i <= r; i++) {
        //op.count();
        auto aux = edges[i];
        int j = i - 1;
        //op.count();
        while (j >= l && compare(edges[j],aux)>0) {
            //op.count(7);
            edges[j + 1] = edges[j];
            j--;
        }
        //op.count();
        edges[j + 1] = aux;
    }
}


int partitionRQS(vector<Edge> &edges, int n, int p, int r) {
    //op.count();
    Edge x = edges[r];
    int i = p - 1;
    for (int j = p; j <= r - 1; j++) {
        // op.count();
        if (compare(edges[j],x)>0) {
            //op.count(3);
            i++;
            swap(edges[i], edges[j]);
        }
    }
    swap(edges[i + 1], edges[r]);
    //op.count(3);
    return i + 1;
}


int randomizedPartition(vector<Edge> &edges, int n, int p, int r) {
    int i = rand() % (r - p + 1) + p;
    //op.count(3);
    swap(edges[r], edges[i]);
    return partitionRQS(edges, n, p, r);
}

void randomizedQuickSort(vector<Edge> &edges, int n, int p, int r) {
    if (p + 10 < r) {
        int q = randomizedPartition(edges, n, p, r);
        randomizedQuickSort(edges, n, p, q - 1);
        randomizedQuickSort(edges, n, q + 1, r);
    } else {
        insertionSort(edges, p, r);
    }
}

int w[MAX_SIZE][MAX_SIZE];

vector<pair<pair<int,int>,int>> kruskal(int n) {
    vector<Edge> edges;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++) {
            if (w[i][j])
                edges.push_back({i, j, w[i][j]});
        }

    randomizedQuickSort(edges, edges.size(),  0, edges.size()-1);
//    for(Edge it: edges)
//        cout<<it.u<<" - "<<it.v<<" ("<<it.weight<<")"<<endl;

    vector<Set *> sets;
    for (int i = 0; i < n; i++) {
        auto *set = new Set;
        MakeSet(&set, n);
        sets.push_back(set);
    }

    vector<pair<pair<int,int>,int>> MST;
    for (Edge it: edges)
        if (Find_Set(sets[it.u], n) != Find_Set(sets[it.v], n)) {
            Union(sets[it.u], sets[it.v], n);
            MST.push_back({{it.u,it.v},it.weight});
        }
    return MST;
}

void demo() {
    srand(time(nullptr));
    int n = 5;
    //Operation op = profiler.createOperation("opDemo", 5);
    auto *a = new Set, *b = new Set, *c = new Set, *d = new Set, *e = new Set, *f = new Set, *g = new Set, *h = new Set, *i = new Set, *j = new Set, *k = new Set;
    MakeSet(&a, n), MakeSet(&b, n), MakeSet(&c, n), MakeSet(&d, n), MakeSet(&e, n), MakeSet(&f, n), MakeSet(&g,n), MakeSet(&h,n), MakeSet(&i, n), MakeSet(&j,n), MakeSet(&k, n);
    Union(b, a, n);
    Union(d, c, n);
    Union(f, e, n);
    Union(h, g, n);
    Union(c, a, n);
    Union(e, a, n);
    Union(g, a, n);
    cout << a->rank << " " << b->rank << " " << c->rank << " " << d->rank << " " << e->rank << endl;
    cout<<endl<<"The rank of h: "<<Find_Set(h,n)->rank<<endl;



    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            w[i][j] = 0;
    for (int i = 1; i < n; i++) {
        w[i - 1][i] = rand() % 10 + 1;
        w[i][i - 1] = w[i - 1][i];
    }
    int remainingEdges = 6;
    while (remainingEdges) {
        int x = rand() % n;
        int y = rand() % n;
        if (x != y && w[x][y] == 0) {
            w[x][y] = rand() % 10 + 1;
            w[y][x] = w[x][y];
            remainingEdges--;
        }
    }
    cout<<endl;
    cout << "The graph:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << w[i][j] << " ";
        cout << endl;
    }

    cout<<endl;
    vector<pair<pair<int,int>,int>> MST = kruskal(n);
    cout << endl << "The minimum spanning tree:" << endl;
    for(pair<pair<int,int>,int> it: MST)
        cout<<it.first.first<<" - "<<it.first.second<<" ("<<it.second<<")"<<endl;
    cout << endl;
}

void perf() {
    srand(time(nullptr));
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                w[i][j] = 0;
        for (int i = 1; i < n; i++) {
            w[i - 1][i] = rand() % 300 + 1;
            w[i][i - 1] = w[i - 1][i];
        }
        int remainingEdges = 3 * n + 1;
        while (remainingEdges) {
            int x = rand() % n;
            int y = rand() % n;
            if (x != y && w[x][y] == 0) {
                w[x][y] = rand() % 300 + 1;
                w[y][x] = w[x][y];
                remainingEdges--;
            }
        }
        kruskal(n);
    }
    profiler.createGroup("Kruskal","opMake","opFindSet","opUnion");
    profiler.showReport();
}

int main() {
    //demo();
    perf();
    return 0;
}