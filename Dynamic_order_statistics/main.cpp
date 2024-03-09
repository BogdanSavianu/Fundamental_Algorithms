/* In aceasta tema am analizat operatiile de management ale unui arbore perfect echilibrat. Acestea sunt cea de construire
a arborelui realizata in O(n), de selectie a al i-lea cel mai mic nod in O(logn) si stergerea tot in O(logn).
    Totusi, desi functia de build are complexitatea de timp O(n), pe grafic pare ca are mai putine operatii decat selectia si stergerea.
 Asta e doar din cauza ca facem n selectii si n stergeri si o singura construire.
*/


#include <bits/stdc++.h>
#include "Profiler.h"
#include <cstdlib>
#include <ctime>
using namespace std;

Profiler profiler("dynamicOrderStatistics");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

typedef struct Node{
    int key,size=0;
    Node *right;
    Node *left;
}PBT;

PBT* buildTree(int left, int right, int n, Operation &op){
    if(left>right){
        return nullptr;
    }
    int mid = (left+right)/2;
    PBT *node = new PBT;
    node->key = mid + 1;
    node->left = buildTree(left,mid-1, n, op);
    node->right = buildTree(mid+1,right, n, op);
    node->size = (node->left ? node->left->size : 0) + (node->right ? node->right->size : 0) + 1;
    op.count(7);
    return node;
}

PBT* osSelect(PBT* tree, int i, int n, Operation &op){
    op.count();
    int r;
    if (tree->left) {
        r = tree->left->size + 1, op.count();
    } else {
        r = 1;
    }
    if(i == r)
        return tree;
    else if(i<r)
        return osSelect(tree->left,i,n,op);
    else return osSelect(tree->right,i-r,n,op);
}

PBT* treeMinimum(PBT* tree,int n, Operation &op){
    op.count();
    while(tree->left)
        tree = tree->left,op.count(2);
    return tree;
}

void osDelete(PBT** toBeDeleted, int key, int n, Operation &op) {
    PBT* temp = nullptr;
    //op.count();
    if(*toBeDeleted == NULL)
        return;
    //op.count();
    if(key < (*toBeDeleted)->key)
        osDelete(&(*toBeDeleted)->left,key,n,op);
    else {
        //op.count();
        if (key > (*toBeDeleted)->key)
            osDelete(&(*toBeDeleted)->right, key, n, op);

        else {
            op.count();
            if (!(*toBeDeleted)->left) {
                op.count(2);
                temp = (*toBeDeleted)->right;
                delete *toBeDeleted;
                *toBeDeleted = temp;
            } else if (!(*toBeDeleted)->right) {
                op.count(3);
                temp = (*toBeDeleted)->left;
                delete *toBeDeleted;
                *toBeDeleted = temp;
            } else {
                op.count();
                temp = treeMinimum((*toBeDeleted)->right, n, op);
                op.count();
                (*toBeDeleted)->key = temp->key;
                osDelete(&(*toBeDeleted)->right, temp->key, n, op);
            }
        }
    }
    if(*toBeDeleted) {
        op.count(3);
        (*toBeDeleted)->size = ((*toBeDeleted)->left ? (*toBeDeleted)->left->size : 0) + ((*toBeDeleted)->right ? (*toBeDeleted)->right->size : 0) + 1;
    }
}

void prettyPrint(PBT* tree, int depth) {
    if (tree) {
        for (int i = 0; i < depth; i++)
            cout << "  ";
        cout << tree->key << " ("<< tree->size<<")"<< endl;

        prettyPrint(tree->left, depth + 1);
        prettyPrint(tree->right, depth+1);
    }
}

void demo(){
    srand(time(nullptr));
    int n = 11;
    Operation opDemo = profiler.createOperation("opDemo", n);
    PBT* tree = buildTree(0,n-1, n, opDemo);
    prettyPrint(tree,0);

    int copyN = n;
    PBT* select = osSelect(tree, rand()%tree->size+1, n,opDemo);
    cout<<endl<<"The tree after the deletion of "<<select->key<<": "<<endl;
    osDelete(&tree, select->key, n, opDemo);
    prettyPrint(tree,0);

    select = osSelect(tree, rand()%tree->size+1, n, opDemo);
    cout<<endl<<"The tree after the deletion of "<<select->key<<": "<<endl;
    osDelete(&tree, select->key, n, opDemo);
    prettyPrint(tree,0);

    select = osSelect(tree, rand()%tree->size+1, n, opDemo);
    cout<<endl<<"The tree after the deletion of "<<select->key<<": "<<endl;
    osDelete(&tree, select->key, n, opDemo);
    prettyPrint(tree,0);

}

void perf(){
    srand(time(nullptr));
    for(int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        Operation opBuild = profiler.createOperation("opBuild", n);
        Operation opSelect = profiler.createOperation("opSelect", n);
        Operation opDelete = profiler.createOperation("opDelete", n);
        for (int test = 0; test <= NR_TESTS; test++) {
            PBT *tree = buildTree(1, n, n, opBuild);
            for(int i = n; i >= 1; i--) {
                int index = rand()%tree->size+1;
                PBT *selected = osSelect(tree, index, n, opSelect);
                osDelete(&tree, selected->key, n, opDelete);
            }
        }

    }
    profiler.divideValues("opBuild", NR_TESTS);
    profiler.divideValues("opSelect", NR_TESTS);
    profiler.divideValues("opDelete", NR_TESTS);

    profiler.createGroup("Dynamic_Order_Statistics","opSelect", "opDelete", "opBuild");

    profiler.showReport();
}

int main() {
    demo();
    //perf();
    return 0;
}