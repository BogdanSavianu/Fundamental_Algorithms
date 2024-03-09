//    Algoritmul pe care l-am implementat in aceasta tema este cel de interclasare a k liste.
//    Prima idee pe care am avut-o pentru a rezolva aceasta problema a fost sa interclasez listele 2 cate 2, iar apoi
//listele rezultate din nou 2 cate 2, dar, desi aceasta metoda este una functionala, foloseste mult spatiu aditional creand
//acele liste intemediare.
//    Pentru a evita aceasta problema am folosit o alta metoda care presupune construirea a unui heap cu k elemente, primul
//element din fiecare lista. Apoi luam varful heap-ului si il punem in lista finala. Apoi trebuie sa punem in locul lui urmatorul
//element tot din aceeasi lista, pentru asta e nevoie de o structura heap unde sa retinem si indexul listei careia ii apartine elementul, apoi
//apelam heapify pe varful heap-ului pentru a restabili proprietatea.
//    In momentul in care am golit una din liste va trebui sa scadem marimea heap-ului si repetam pasii pana cand am golit cu totul heap-ul
//sau pana cand mai avem 2 liste ramase si le putem interclasa direct in lista finala cu algoritmul clasic.
//    Complexitatea acestui algoritm este O(n*lgk), iar spatiul aditional este O(k).


#include <bits/stdc++.h>
#include "Profiler.h"
using namespace std;

Profiler profiler("merge_k_lists");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 15



typedef struct Node{
    int key;
    int order;
    Node* next;
}node;


node* createNode(int key, int order){
    node* x = new node;
    x->key = key;
    x->order = order;
    x->next = NULL;
    return x;
}

void insertFinalAlg(node** first, int key, int order,int n,Operation &opComp, Operation &opAttr){
    node* newNode = createNode(key, order);
    opComp.count();
    if(*first==NULL) {
        opAttr.count();
        *first = newNode;
    }
    else{
        opAttr.count();
        node* q = *first;
        while(q->next)
            q = q->next;
        q->next= newNode;
    }
}

void insertFirstGenerate(node** first, int key, int order){
    node* newNode = createNode(key, order);
    if(*first==NULL) {
        *first = newNode;
    }
    else{
        newNode -> next = *first;
        *first = newNode;
    }
}


void deleteFirst(node** first, int n, Operation &opComp, Operation &opAttr){
    opAttr.count();
    node* toBeDeleted = *first;
    opAttr.count();
    *first = (*first)->next;
    delete(toBeDeleted);
}

void printList(node* first){
    node* x = first;
    cout<<"Index list: "<<x->order<<", list: ";
    while(x)
    {
        cout << x->key << " ";
        x=x->next;
    }
    cout<<endl;
}

void generatePartition(int a[], int n, int k){
    int differences[k+1];
    FillRandomArray(differences,k,0,n-1,1,1);
    differences[0]=0, differences[k]=n;
    for(int i=1; i<=k; i++)
        a[i-1]=differences[i]-differences[i-1];
}

void turnArrayToList(int a[], int n, int order, node** first){
    for(int i = 0; i<n; i++)
        insertFirstGenerate(first, a[i], order);
}

void generateKRandomLists(node* lists[], int k, int partitions[], int n){
    int aux[MAX_SIZE];
    generatePartition(partitions, n, k);
    for(int i=0;i<k;i++){
        FillRandomArray(aux,partitions[i],10,50000, 0,2);
        turnArrayToList(aux,partitions[i], i, &lists[i]);
    }
}

void mergeTwoLists(node* abra, node* cadabra, node** abracadabra,int n, Operation &opComp, Operation &opAttr) {
    opComp.count();
    while (abra && cadabra) {
        opComp.count(2);
        if (abra->key < cadabra->key) {
            insertFinalAlg(abracadabra,abra->key,abra->order,n,opComp,opAttr);
            opAttr.count();
            abra = abra->next;
        } else {
            insertFinalAlg(abracadabra,cadabra->key,cadabra->order,n,opComp,opAttr);
            opAttr.count();
            cadabra = cadabra->next;
        }
    }

    while (abra) {
        insertFinalAlg(abracadabra,abra->key,abra->order,n,opComp,opAttr);
        opAttr.count();
        abra = abra->next;
    }

    while (cadabra) {
        insertFinalAlg(abracadabra,cadabra->key,cadabra->order,n,opComp,opAttr);
        opAttr.count();
        cadabra = cadabra->next;
    }
}

void minHeapifyBU(vector<pair<int,int>> &a,int n, int &heapSize, int i,Operation &opComp, Operation &opAttr)
{
    int l=i*2+1,r=i*2+2,min=i;
    if(l<heapSize){
        opComp.count();
        if(a[l].first<a[min].first)
            min = l;
        else min = i;
    } else min=i;
    if(r<heapSize){
        opComp.count();
        if(a[r].first<a[min].first)
            min = r;
    }
    if(min!=i)
    {
        opAttr.count(3);
        swap(a[i],a[min]);
        minHeapifyBU(a,n,heapSize,min,opComp,opAttr);
    }
}

void buildMinHeap(vector<pair<int,int>> &a,int n, Operation &opComp, Operation &opAttr)
{
    int heapSize = n;
    for(int i = n/2-1;i>=0;i--)
        minHeapifyBU(a,n,heapSize,i, opComp, opAttr);
}

pair<int, int> extractMin(vector<pair<int,int>> &a){
    return a[0];
}

void mergeKRandomLists(node* lists[], int k, Node** final, int n, int &heapSize,Operation &opComp, Operation &opAttr){
    int j;
    heapSize = k;
    vector<pair<int, int>> heap;
    for(int i=0;i<k;i++) {
        opAttr.count(2);
        heap.emplace_back(lists[i]->key, i);
    }

    buildMinHeap(heap,k, opComp, opAttr);

        while (heapSize > 0) {
            for(j=0; j<n; j++) {
            insertFinalAlg(final,heap[0].first,heap[0].second,n,opComp,opAttr);
            deleteFirst(&lists[heap[0].second],n,opComp,opAttr);
            opComp.count();
            if (lists[heap[0].second]) {
                opAttr.count(2);
                heap[0].first = lists[heap[0].second]->key;
                heap[0].second = lists[heap[0].second]->order;
            }
            else {
                opAttr.count();
                heap[0] = heap[heapSize - 1];
                heapSize--;
            }
            minHeapifyBU(heap, n, heapSize, 0,opComp,opAttr);
        }
    }
}

void demo(){
    vector<pair<int,int>> a={make_pair(1,0),
                             make_pair(5,0),
                             make_pair(2, 0),
                             make_pair(3,0),
                             make_pair(34, 0),
                             make_pair(21, 0),
                             make_pair(2, 0),
                             make_pair(54,0),
                             make_pair(32,0),
                             make_pair(17, 0),
                             make_pair(19, 0),
                             make_pair(10,0)};
    int n = sizeof(a)/sizeof(a[0]);

    int k=4,partitions[k];
    n=25;
    node* lists[6]={NULL};
    generateKRandomLists(lists,k,partitions,n);
    cout<<endl;
    for(int i=0;i<k;i++)
        printList(lists[i]);

    cout<<endl<<"Merged list: ";

    k=4,n=25;

    Operation opComp = profiler.createOperation("compTest", n);
    Operation opAttr = profiler.createOperation("attrTest",n);

    vector<pair<int,int>> merged;
    int heapSize;
    Node* final = NULL;
    mergeKRandomLists(lists,k,&final,n,heapSize,opComp,opAttr);
    Node* q=final;
    while(q)
        cout<<q->key<<" ",q=q->next;
    cout<<endl;
    node *listA=NULL,*listB=NULL;
    insertFinalAlg(&listA,1,1,100,opComp,opAttr);
    insertFinalAlg(&listA,3,1,100,opComp,opAttr);
    insertFinalAlg(&listA,5,1,100,opComp,opAttr);
    insertFinalAlg(&listA,7,1,100,opComp,opAttr);

    insertFinalAlg(&listB,2,1,100,opComp,opAttr);
    insertFinalAlg(&listB,4,1,100,opComp,opAttr);
    insertFinalAlg(&listB,6,1,100,opComp,opAttr);
    insertFinalAlg(&listB,8,1,100,opComp,opAttr);
    node *listC=NULL, *list1=listA, *list2=listB;

    cout<<endl<<"Exemple of merging 2 lists: "<<endl<<"List 1: ";
    while(list1)
        cout<<list1->key<<" ",list1=list1->next;
    cout<<endl<<"List 2: ";
    while(list2)
        cout<<list2->key<<" ",list2=list2->next;
    mergeTwoLists(listA,listB,&listC,100,opComp,opAttr);
    cout<<endl<<"Merged list: ";
    while(listC)
        cout<<listC->key<<" ",listC=listC->next;
}

void perf() {
    int k[] = {5, 10, 100}, n;

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        Operation opComp1 = profiler.createOperation("comp-k=5", n);
        Operation opAttr1 = profiler.createOperation("attr-k=5", n);
        for (int j = 0; j < NR_TESTS; j++) {
            int *partitions = (int *) malloc(n * sizeof(int));
            int heapSize = n;
            Node *lists[101] = {NULL};
            Node *merged = NULL;
            generateKRandomLists(lists, k[0], partitions, n);
            mergeKRandomLists(lists, k[0], &merged, n, heapSize, opComp1, opAttr1);
        }
    }
    profiler.divideValues("comp-k=5",NR_TESTS);
    profiler.divideValues("attr-k=5",NR_TESTS);
    profiler.addSeries("Total-k=5", "comp-k=5", "attr-k=5");

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        Operation opComp2 = profiler.createOperation("comp-k=10", n);
        Operation opAttr2 = profiler.createOperation("attr-k=10", n);
        for (int j = 0; j < NR_TESTS; j++) {
            int *partitions = (int *) malloc(n * sizeof(int));
            int heapSize = n;
            Node *lists[101] = {NULL};
            Node *merged = NULL;
            generateKRandomLists(lists, k[1], partitions, n);
            mergeKRandomLists(lists, k[1], &merged, n, heapSize, opComp2, opAttr2);
        }
    }
    profiler.divideValues("comp-k=10", NR_TESTS);
    profiler.divideValues("attr-k=10", NR_TESTS);
    profiler.addSeries("Total-k=10", "comp-k=10", "attr-k=10");

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        Operation opComp3 = profiler.createOperation("comp-k=100", n);
        Operation opAttr3 = profiler.createOperation("attr-k=100", n);
        for (int j = 0; j < NR_TESTS; j++) {
            int *partitions = (int *) malloc(n * sizeof(int));
            int heapSize = n;
            Node *lists[101] = {NULL};
            Node *merged = NULL;
            generateKRandomLists(lists, k[2], partitions, n);
            mergeKRandomLists(lists, k[2], &merged, n, heapSize, opComp3, opAttr3);
        }
    }
    profiler.divideValues("comp-k=100", NR_TESTS);
    profiler.divideValues("attr-k=100", NR_TESTS);
    profiler.addSeries("Total-k=100", "comp-k=100", "attr-k=100");

    profiler.createGroup("Assignments", "attr-k=5", "attr-k=10", "attr-k=100");
    profiler.createGroup("Comparisons", "comp-k=5", "comp-k=10", "comp-k=100");
    profiler.createGroup("Total", "Total-k=5", "Total-k=10", "Total-k=100");

    profiler.reset("PARTEA 2");

    n = MAX_SIZE;
    for (int i = 10; i <= 500; i += 10) {
        for(int j=0;j<NR_TESTS;j++) {
            Operation opComp4 = profiler.createOperation("comp-n=10000", i);
            Operation opAttr4 = profiler.createOperation("attr-n=10000", i);
            int partitions[501] = {0}, heapSize = n;
            Node *lists[501] = {NULL};
            Node *merged = NULL;
            generateKRandomLists(lists, i, partitions, n);
            mergeKRandomLists(lists, i, &merged, n, heapSize, opComp4, opAttr4);
        }
    }
    profiler.divideValues("comp-n=10000",NR_TESTS);
    profiler.divideValues("attr-n=10000",NR_TESTS);
    profiler.addSeries("Total-n=10000", "comp-n=10000", "attr-n=10000");
    profiler.createGroup("Total n=10000", "Total-n=10000", "comp-n=10000", "attr-n=10000");
    profiler.showReport();
}

int main() {
    demo();
    //perf();
    return 0;
}
