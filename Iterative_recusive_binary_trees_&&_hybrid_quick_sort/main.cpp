/*  Pentru prima parte a temei am implementat parcurgerea iterativa a arborilor, care are avantajul ca nu foloseste stiva
pentru apeluri recursive. Complexitatea este O(n).
    Apoi am implementat quick sort hibrid cu pragul=30 si l-am comparat cu quick sort clasic. Am observat ca numarul de opratii
este asemanator, dar timpul de rulare este aproximativ de 2 ori mai rapid pentru ca nu mai face apelurile recursive pentru
numar mic de elemente.
    Am incercat sa aflu si pragul optim pentru a introduce insertion sort prin incercarea tuturor pragurilor de la 0 la 100
si am descoperit ca in jur de n=10 se fac cele mai putine operatii.
 */


#include <bits/stdc++.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 100
using namespace std;

Profiler profiler("assignment8");

typedef struct Node {
    int key;
    Node *left, *right, *parent;
} Tree;

void insert(Tree **tree, int key) {
    Tree *y = nullptr;
    Tree *x = *tree;
    while (x) {
        y = x;
        if (key < x->key)
            x = x->left;
        else x = x->right;
    }
    if (y == nullptr)
        *tree = new Tree(key, nullptr, nullptr, nullptr);
    else if (key < y->key)
        y->left = new Tree(key, nullptr, nullptr, y);
    else y->right = new Tree(key, nullptr, nullptr, y);
}

void recursiveWalkDemo(Tree *tree) {
    if (tree) {
        recursiveWalkDemo(tree->left);
        cout << tree->key << " ";
        recursiveWalkDemo(tree->right);
    }
}


void iterativeWalkDemo(Tree *tree) {
    int d = 1;
    auto node = tree;
    do {
        if (d == 1) {
            if (node->left)
                node = node->left;
            else d = 2;
        }
        if (d == 2) {
            cout << node->key << " ";
            if (node->right) {
                node = node->right;
                d = 1;
            } else d = 3;
        }
        if (d == 3) {
            if (node->parent) {
                if (node == node->parent->left)
                    d = 2;
                node = node->parent;
            }
        }
    } while (node != tree || d != 3);
}

void recursiveWalk(Tree *tree, Operation &op) {
    if (tree) {
        recursiveWalk(tree->left, op);
        op.count(); //ce sens are ????????
        recursiveWalk(tree->right, op);
    }
}


void iterativeWalk(Tree *tree, Operation &op) {
    int d = 1;
    Tree *node = tree;
    do {
        if (d == 1) {
            if (node->left)
                node = node->left;
            else d = 2;
        }
        if (d == 2) {
            op.count();
            if (node->right) {
                node = node->right;
                d = 1;
            } else d = 3;
        }
        if (d == 3) {
            if (node->parent) {
                if (node == node->parent->left)
                    d = 2;
                node = node->parent;
            }
        }
    } while (node != tree || d != 3);
}

void insertion(int v[], int l, int r, Operation &op) {
    for (int i = l + 1; i <= r; i++) {
        op.count();
        int aux = v[i];
        int j = i - 1;
        while (v[j] > aux && j >= l) {
            op.count();
            op.count();
            v[j + 1] = v[j], j--;
        }
        op.count();
        op.count();
        v[j + 1] = aux;
    }
}


int partitionRQS(int a[], int n, int p, int r, Operation &op) {
    op.count();
    int x = a[r];
    int i = p - 1;
    for (int j = p; j <= r - 1; j++) {
        op.count();
        if (a[j] <= x) {
            i++;
            op.count(3);
            swap(a[i], a[j]);
        }
    }
    op.count(3);
    swap(a[i + 1], a[r]);
    return i + 1;
}

int getRandomNumber(int low, int high, std::mt19937 &generator) {
    std::uniform_int_distribution<int> dist(low, high);
    return dist(generator);
}

int randomizedPartition(int a[], int n, int p, int r, Operation &op, std::mt19937 &generator) {
    int i = getRandomNumber(p, r, generator);
    op.count(3);
    swap(a[r], a[i]);
    return partitionRQS(a, n, p, r, op);
}

void randomizedQuickSort(int a[], int n, int p, int r, int threshold, Operation &op, std::mt19937 &generator) {
    if (p + threshold < r) {
        int q = randomizedPartition(a, n, p, r, op, generator);
        randomizedQuickSort(a, n, p, q - 1, threshold, op, generator);
        randomizedQuickSort(a, n, q + 1, r, threshold, op, generator);
    } else {
        insertion(a, p, r, op);
    }
}

void insertionRunTime(int v[], int l, int r) {
    for (int i = l + 1; i <= r; i++) {
        int aux = v[i];
        int j = i - 1;
        while (v[j] > aux && j >= l) {
            v[j + 1] = v[j], j--;
        }
        v[j + 1] = aux;
    }
}

int partitionRQSRunTime(int a[], int n, int p, int r) {
    int x = a[r];
    int i = p - 1;
    for (int j = p; j <= r - 1; j++) {
        if (a[j] <= x) {
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[r]);
    return i + 1;
}

int randomizedPartitionRunTime(int a[], int n, int p, int r, std::mt19937 &generator) {
    int i = getRandomNumber(p, r, generator);
    swap(a[r], a[i]);
    return partitionRQSRunTime(a, n, p, r);
}

void randomizedQuickSortRunTime(int a[], int n, int p, int r, int threshold, std::mt19937 &generator) {
    if (p + threshold < r) {
        int q = randomizedPartitionRunTime(a, n, p, r, generator);
        randomizedQuickSortRunTime(a, n, p, q - 1, threshold, generator);
        randomizedQuickSortRunTime(a, n, q + 1, r, threshold, generator);
    } else {
        insertionRunTime(a, p, r);
    }
}

void demoWalks() {
    Tree *tree = nullptr;
    int array[] = {4, 5, 2, 3, 17, 23, 1, 8, 9, 10, 20, 19};
    int n = sizeof(array) / sizeof(array[0]);
    Operation opDemo = profiler.createOperation("random-quick-sort-comp", n);
    for (int i = 0; i < n; i++)
        insert(&tree, array[i]);
    cout << "Recursive walk: ";
    recursiveWalkDemo(tree);
    cout << endl << "Iterative walk: ";
    iterativeWalkDemo(tree);
    cout << endl;
}

void demoSort() {
    std::mt19937 generator(1);
    int a[] = {65, 43, 12, 34, 78, 56, 45, 34, 778, 890, 76, 56, 43, 4, 5, 6, 3};
    int len = sizeof(a) / sizeof(a[0]);
    Operation opDemo = profiler.createOperation("opDemo", len);
    //insertion(a, 0, len, opDemo);
    randomizedQuickSort(a, len, 0, len - 1, 3, opDemo, generator);
    for (int i = 0; i < len; i++)
        cout << a[i] << " ";
}

void performanceWalks() {
    int a[MAX_SIZE];
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        Operation opIt = profiler.createOperation("iterative_walk", n);
        Operation opRec = profiler.createOperation("recursive_walk", n);
        FillRandomArray(a, n, 10, 50000, true, 0);
        Tree *tree = nullptr;
        for (int j = 0; j < n; j++)
            insert(&tree, a[j]);
        iterativeWalk(tree, opIt);
        recursiveWalk(tree, opRec);
    }
    profiler.createGroup("Binary_tree_walk", "recursive_walk", "iterative_walk");
    profiler.showReport();
}

void performanceQSOperations() {
    int a[MAX_SIZE],b[MAX_SIZE];
    std::mt19937 generator(0);
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        Operation op1 = profiler.createOperation("quick_sort", n);
        Operation op2 = profiler.createOperation("quick_sort_hybrid", n);
        for (int test = 0; test < NR_TESTS; test++) {
            FillRandomArray(a, n, 10, 50000, false, 0);
            CopyArray(b,a,n);
            randomizedQuickSort(a, n, 0, n - 1, 0, op1, generator);
            randomizedQuickSort(b,n,0,n-1,30,op2,generator);
        }
    }
    profiler.divideValues("quick_sort", NR_TESTS);
    profiler.divideValues("quick_sort_hybrid", NR_TESTS);
    profiler.createGroup("Classic_VS_Hybrid","quick_sort","quick_sort_hybrid");
    profiler.showReport();
}

void performanceQSvsHybridRunTime() {
    int a[MAX_SIZE], b[MAX_SIZE],c[MAX_SIZE];


    for (int n=STEP_SIZE; n <= MAX_SIZE; n+=STEP_SIZE) {
        FillRandomArray(a, n, 10, 50000, false, 0);
        std::mt19937 generator(56);
        profiler.startTimer("Classic", n);
        for (int test = 0; test < NR_TESTS; test++) {
            CopyArray(b, a, n);
            srand(56);
            randomizedQuickSortRunTime(a, n, 0, n-1, 0, generator);
        }
        profiler.stopTimer("Classic", n);

        profiler.startTimer("Hybrid", n);
        for (int test = 0; test < NR_TESTS; test++) {
            CopyArray(c, a, n);
            srand(56);
            randomizedQuickSortRunTime(c, n, 0, n-1, 30, generator);
        }
        profiler.stopTimer("Hybrid", n);
    }
    profiler.createGroup("Classic_VS_Hybrid","Classic","Hybrid");
    profiler.showReport();
}

void performanceQSOperationsVar() {
    int a[MAX_SIZE], b[MAX_SIZE];
    FillRandomArray(a, 10000, 10, 50000, false, 0);

    for (int threshold = 0; threshold <= 100; threshold++) {
        Operation op = profiler.createOperation("Variable_threshold", threshold);
        std::mt19937 generator(56);
        for (int test = 0; test < NR_TESTS; test++) {
            FillRandomArray(a, 10000, 10, 50000, false, 0);
            srand(56);
            randomizedQuickSort(a, 10000, 0, 9999, threshold, op, generator);
        }
    }
    profiler.divideValues("Variable_threshold", NR_TESTS);
    profiler.showReport();
}

void performanceQSRunTime() {
    int a[MAX_SIZE], b[MAX_SIZE];
    FillRandomArray(a, 10000, 10, 50000, false, 0);

    for (int threshold = 0; threshold <= 1000; threshold++) {
        std::mt19937 generator(56);
        profiler.startTimer("performanceQSRunTime", threshold);
        for (int test = 0; test < NR_TESTS; test++) {
            CopyArray(b, a, 10000);
            srand(56);
            randomizedQuickSortRunTime(b, 10000, 0, 9999, threshold, generator);
        }
        profiler.stopTimer("performanceQSRunTime", threshold);
    }
    profiler.showReport();
}

int main() {
//    demoWalks();
    demoSort();
//    performanceWalks();
//    performanceQSOperations();
//    performanceQSvsHybridRunTime();
//    performanceQSOperationsVar();
//    performanceQSRunTime();
    return 0;
}
