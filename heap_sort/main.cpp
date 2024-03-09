
/*Am implementat heap sort prin implementarile bottom-up si top-down.
Am observat ca pe cazul general abordarea bottom-up este cam de 2 ori mai rapida, in schimb pe worst case diferenta este mult mai accentuata.
Construirea heap-ului cu bottom are complexitatea O(n), in timp ce cea cu top-down are O(nlogn).*/


#include <iostream>
#include "Profiler.h"
using namespace std;

Profiler p("heap");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

void maxHeapifyBU(int a[],int n, int heapSize, int i)
{
    Operation opComp = p.createOperation("bottom-up-comp", n);
    Operation opAttr = p.createOperation("bottom-up-attr",n);
    int l=i*2+1,r=i*2+2,largest=i;
    if(l<heapSize){
        opComp.count();
        if(a[l]>a[largest])
            largest = l;
        else largest = i;
    } else largest=i;
    if(r<heapSize){
        opComp.count();
        if(a[r]>a[largest])
            largest = r;
    }
    if(largest!=i)
    {
        opAttr.count(3);
        swap(a[i],a[largest]);
        maxHeapifyBU(a,n,heapSize,largest);
    }
}

void buildMaxHeap(int a[],int n)
{
    int heapSize = n;
    for(int i = n/2-1;i>=0;i--)
        maxHeapifyBU(a,n,heapSize,i);
}

void heapSortBU(int a[], int n, int heapSize)
{
    Operation opAttr = p.createOperation("bottom-up-attr",n);
    buildMaxHeap(a,n);
    for(int i=n-1;i>=1;i--)
    {
        //opAttr.count(3);
        swap(a[0],a[i]);
        heapSize--;
        maxHeapifyBU(a,n,heapSize,0);
    }
}

//acum incepe top-down


void maxHeapifyTD(int a[],int n, int heapSize, int i)
{
    Operation opComp = p.createOperation("top-down-comp", n);
    Operation opAttr = p.createOperation("top-down-attr",n);
    int l=i*2+1,r=i*2+2,largest=i;
    if(l<heapSize){
        opComp.count();
        if(a[l]>a[largest])
            largest = l;
        else largest = i;
    }else largest = i;
    if(r<heapSize){
        opComp.count();
        if(a[r]>a[largest])
            largest = r;
    }
    if(largest!=i)
    {
        opAttr.count(3);
        swap(a[i],a[largest]);
        maxHeapifyTD(a,n,heapSize,largest);
    }
}

int parent(int i){
    if(i%2)
        return i/2;
    return i/2-1;
}
void heapIncreaseKey(int a[],int n, int i,int key)
{
    Operation opComp = p.createOperation("top-down-comp", n);
    Operation opAttr = p.createOperation("top-down-attr",n);
    opComp.count();
    if(key < a[i])
        perror("new key is smaller than current key");
    opAttr.count();
    a[i]=key;
    while(i>0 && a[parent(i)]<a[i])
    {
        opComp.count();
        opAttr.count(3);
        swap(a[i],a[parent(i)]);
        i=parent(i);
    }
    if(i)
        opComp.count();
}

void maxHeapInsert(int a[], int n, int &heapSize, int key)
{
    Operation opAttr = p.createOperation("top-down-attr",n);
    heapSize++;
    a[heapSize] = INT_MIN;
    opAttr.count();
    heapIncreaseKey(a,n,heapSize,key);
}

void buildMaxHeapInsertion(int a[], int n)
{
    int heapSize = 0;
    for(int i=1;i<n;i++)
        maxHeapInsert(a,n, heapSize,a[i]);
}

void heapSortTD(int a[],int n, int heapSize)
{
    Operation opComp = p.createOperation("top-down-comp", n);
    Operation opAttr = p.createOperation("top-down-attr",n);
    buildMaxHeapInsertion(a,n);
    for(int i=n-1;i>=1;i--)
    {
        //opAttr.count(3);
        swap(a[0],a[i]);
        heapSize--;
        maxHeapifyTD(a,n,heapSize,0);
    }
}


void demo()
{
    int a[]={9, 7, 2, 6,1,45, 34, 23, 12, 67};
    int a2[]={9, 7, 2, 6,1,45, 34, 23, 12, 67};
    int n=sizeof(a)/sizeof(a[0]);
    buildMaxHeap(a,n);
    for(int i=0;i<n;i++)
        cout<<a[i]<<" ";
    cout<<endl;
    heapSortBU(a,n,n);
    for(int i=0;i<n;i++)
        cout<<a[i]<<" ";
    cout<<endl<<endl;


    buildMaxHeapInsertion(a2,n);
    for(int i=0;i<n;i++)
        cout<<a2[i]<<" ";
    cout<<endl;
    heapSortTD(a2,n,n);
    for(int i=0;i<n;i++)
        cout<<a2[i]<<" ";
    cout<<endl;
}

void perf(int order){
    int v1[MAX_SIZE], v2[MAX_SIZE],n;
    if(order==0) {
        for (n = STEP_SIZE; n <= MAX_SIZE; n += 100)
            for (int k = 0; k < NR_TESTS; ++k) {
                FillRandomArray(v1, n + 1, 10, 50000, 0, order);
                for (int i = 0; i < n; i++)
                    v2[i] = v1[i];
                buildMaxHeap(v1, n);
                buildMaxHeapInsertion(v2, n);
            }
        p.divideValues("bottom-up-attr", NR_TESTS);
        p.divideValues("bottom-up-comp", NR_TESTS);
        p.addSeries("bottom-up", "bottom-up-attr", "bottom-up-comp");

        p.divideValues("top-down-attr", NR_TESTS);
        p.divideValues("top-down-comp", NR_TESTS);
        p.addSeries("top-down", "top-down-attr", "top-down-comp");

        p.createGroup("Assignments", "bottom-up-attr", "top-down-attr");
        p.createGroup("Comparisons", "bottom-up-comp", "top-down-comp");
        p.createGroup("Total", "bottom-up", "top-down");
    }
    else{
        for (n = STEP_SIZE; n <= MAX_SIZE; n += 100) {
            FillRandomArray(v1, n + 1, 10, 50000, 0, order);
            for (int i = 0; i < n; i++)
                v2[i] = v1[i];
            buildMaxHeap(v1, n);
            buildMaxHeapInsertion(v2, n);
        }
        p.addSeries("bottom-up", "bottom-up-attr", "bottom-up-comp");
        p.addSeries("top-down", "top-down-attr", "top-down-comp");

        p.createGroup("Assignments", "bottom-up-attr", "top-down-attr");
        p.createGroup("Comparisons", "bottom-up-comp", "top-down-comp");
        p.createGroup("Total", "bottom-up", "top-down");
    }
}

void perf_all()
{
    perf(UNSORTED);
    p.reset("Worst");
    perf(1);
    p.showReport();
}

int main() {
    demo();
    //perf_all();
    return 0;
}
