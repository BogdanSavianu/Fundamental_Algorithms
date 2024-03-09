/*  In aceasta tema am avut ca date de intrare reprezentarea unui arbore prin vector de parinti. Acest arbore l-am transformat
in reprezentare multicai. Pentru asta am inceput de la indexul radacinii si am parcurs vectorul de parinti pentru a gasi
copiii acelui nod (indecsii unde parintele era radacina) si, pentru fiecare copil, am creat un nod pe care l-am bagat in vectorul
de copii al radacinii si apoi am apelat recursiv functia pentru acel nod.
    Apoi am facut transformarea din reprezentarea multicai in reprezentarea binara, unde fiecare nod are pointer catre primul
copil si urmatorul frate. Acest lucru l-am facut incepand cu radacina arborelui multicai si am creat un nou nod pentru reprezentarea binara.
Am parcurs vectorul de copii si, daca acesta este primul copil al radacinii, l-am introdus ca copil, altfel ca fratele nodului curent
si am apelat recursiv functia pentru fiecare copil.
    Pentru toate cele 3 reprezentari am facut si functia corespunzatoare de pretty print.
    Ambele transformari au fost facute in O(n) fara spatiu auxiliar.
*/


#include <bits/stdc++.h>
#include <vector>
using namespace std;

struct Tree1{
    int key, parent;
};

struct Tree2{
    int key;
    vector<Tree2*> children;
};

typedef struct Node{
    int key;
    Node* brother,*child;
}Tree3;

void prettyPrintR1(Tree1 tree[],int n, int key, int depth){
    for (int j = 0; j < depth; j++) {
        cout << "  ";
    }
    cout << key  << endl;
    for (int i = 0; i < n; i++) {
        if (tree[i].parent == key) {
            prettyPrintR1(tree, n, tree[i].key, depth + 1);
        }
    }
}

void T1(Tree1 tree1[], Tree2 **root, int n, int index){
    (*root)->key = index+1;
    for(int i=0; i<n; i++)
        if (tree1[i].parent == index + 1) {
            Tree2 *child = new Tree2;
            (*root)->children.push_back(child);
            T1(tree1, &child, n, i);
        }
}

void prettyPrintR2(Tree2* tree2, int depth){
    if(tree2) {
        for (int i = 0; i < depth; i++)
            cout << "  ";
        cout << tree2->key << endl;
        for (Tree2 *child: tree2->children) {
            prettyPrintR2(child, depth + 1);
        }
    }
}

void T2(Tree2 *tree2, Tree3** tree3){
    if(tree2) {
        (*tree3)->key = tree2->key;
        (*tree3)->brother = nullptr;
        (*tree3)->child = nullptr;
        Tree3 *currentChildT3 = nullptr;
        for(Tree2* childT2: tree2->children){
            Tree3 *childT3 = new Tree3;
            if(!currentChildT3)
                (*tree3)->child = childT3;
            else currentChildT3->brother = childT3;
            currentChildT3 = childT3;
            T2(childT2,&childT3);
        }
    }
}

void prettyPrintR3(Tree3* tree3, int depth) {
    if (tree3) {
        for (int i = 0; i < depth; i++)
            cout << "  ";
        cout << tree3->key << endl;

        prettyPrintR3(tree3->child, depth + 1);
        prettyPrintR3(tree3->brother, depth);
    }
}

void demo(){
    Tree1 tree1[]={
            1,2,
            2,7,
            3,5,
            4,2,
            5,7,
            6,7,
            7,-1,
            8, 5,
            9,2,
    };
    cout<<"The first tree:"<<endl;
    prettyPrintR1(tree1,9,7,0);

    Tree2 *tree2=new Tree2;
    int n=9;
    T1(tree1,&tree2,n,6);
    cout<<endl<<endl;

    cout<<"The second tree:"<<endl;
    prettyPrintR2(tree2,0);
    cout<<endl<<endl;

    Tree3 *root = new Tree3;
    T2(tree2, &root);
    cout<<"The third tree:"<<endl;
    prettyPrintR3(root,0);
}
int main() {
    demo();
    return 0;
}