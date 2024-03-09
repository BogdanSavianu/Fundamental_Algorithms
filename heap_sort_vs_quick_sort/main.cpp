/* Am implementat quick sort cu pivotul la final si pivot random, heap sort cu metoda bottom-up si sortarea prin inserare binara, atat iterativ,
cat si recursiv. Celor 2 din urma le-am comparat si timpul de executie.
 Am observat ca quick sort are cazul nefavorabil atunci cand pivotul este cel mai mic sau cel mai mare element deoarece partitia
 se face foarte inegal (1 element si toate celelalte dincolo) si se ajunge la complexitate O(n^2). Cazul cel mai fericit e cand
 pivotul este mediana sau un element foarte apropiat, astfel impartindu-se mereu sirul in 2 parti egale. Pentru a putea folosi acest lucru,
 am implementat o varianta cu pivotul fiind elementul din mijloc si am luat vectori sortati crescator. Cand am facut graficul pt cazul favorabil,
 am ales sa reprezint varianta cu pivot random separat pentru ca facea mai multe operatii decat celelalte 2 sortari.

 Apoi am comparat sortarea prin inserare binara implementate iterativ. Numarul de operatii ar trebui sa fie asemanator, insa din cauza
 faptului ca recursivitatea acceseaza stiva pt a plasa argumentele si variabilele locale, a luat considerabil mai mult timp decat varianta iterativa.
 */


#include <iostream>
#include <time.h>
#include <cstdlib>
#include "Profiler.h"
using namespace std;

Profiler profiler("heap");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

void maxHeapifyBU(int a[],int n, int heapSize, int i)
{
    Operation opComp = profiler.createOperation("heap-sort-comp", n);
    Operation opAttr = profiler.createOperation("heap-sort-attr",n);
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
    Operation opAttr = profiler.createOperation("heap-sort-attr",n);
    buildMaxHeap(a,n);
    for(int i=n-1;i>=1;i--)
    {
        opAttr.count(3);
        swap(a[0],a[i]);
        heapSize--;
        maxHeapifyBU(a,n,heapSize,0);
    }
}




int partitionQS(int a[], int n, int p, int r){
    Operation opComp = profiler.createOperation("quick-sort-comp", n);
    Operation opAttr = profiler.createOperation("quick-sort-attr",n);
    opAttr.count();
    int x = a[r];
    int i = p - 1;
    for(int j = p; j<=r-1; j++) {
        opComp.count();
        if (a[j] <= x) {
            i++;
            opAttr.count(3);
            swap(a[i], a[j]);
        }
    }
    opAttr.count(3);
    swap(a[i+1],a[r]);
    return i+1;
}

void quickSort(int a[], int n, int p, int r){
    if(p<r){
        int q = partitionQS(a,n,p,r);
        quickSort(a, n, p,q-1);
        quickSort(a, n,q+1, r);
    }
}

int partitionQSBest(int a[], int n, int p, int r){
    Operation opComp = profiler.createOperation("quick-sort-comp", n);
    Operation opAttr = profiler.createOperation("quick-sort-attr",n);
    opAttr.count();
    int x = a[(r+p)/2], i=p, j=r;
    opAttr.count(3);
    swap(a[r], a[(p+r)/2]);
    do{
        while(a[i] < x)
            i++,opComp.count();
        opComp.count();
        while(a[j] > x)
            j--,opComp.count();
        opComp.count();
        if(i <= j){
            opAttr.count(3);
            swap(a[i],a[j]);
            i++,j--;
        }
    }while(j >= i);
    return j;
}

void quickSortBest(int a[], int n, int p, int r){
    if(p<r){
        int q = partitionQSBest(a,n,p,r);
        quickSortBest(a, n, p,q);
        quickSortBest(a, n,q+1, r);
    }
}

int partitionRQS(int a[], int n, int p, int r){
    Operation opComp = profiler.createOperation("random-quick-sort-comp", n);
    Operation opAttr = profiler.createOperation("random-quick-sort-attr", n);
    opAttr.count();
    int x = a[r];
    int i = p - 1;
    for(int j = p; j<=r-1; j++) {
        opComp.count();
        if (a[j] <= x) {
            i++;
            opAttr.count(3);
            swap(a[i], a[j]);
        }
    }
    opAttr.count(3);
    swap(a[i+1],a[r]);
    return i+1;
}

int randomizedPartition(int a[], int n, int p, int r){
    Operation opComp = profiler.createOperation("random-quick-sort-comp", n);
    Operation opAttr = profiler.createOperation("random-quick-sort-attr",n);
    int i = rand()%(r-p+1)+p;
    opAttr.count(3);
    switch (a[r],a[i]);
    return partitionRQS(a, n, p, r);
}

void randomizedQuickSort(int a[], int n, int p, int r){
    if(p<r){
        int q = randomizedPartition(a, n, p, r);
        randomizedQuickSort(a, n, p,q-1);
        randomizedQuickSort(a, n,q+1,r);
    }
}

void binaryInsertion(int v[],int n)
{
    Operation opComp = profiler.createOperation("insertion-bin-comp", n);
    Operation opAttr = profiler.createOperation("insertion-bin-attr",n);
    for(int i=1;i<n;i++)
    {
        int st=0,dr=i-1,m=(st+dr)/2;
        while(st<=dr)
        {
            opComp.count();
            if(v[i]<v[m])
                dr=m-1;
            else st=m+1;
            m=(st+dr)/2;
        }
        opAttr.count();
        int aux=v[i];
        for(int j=i;j>st;j--)
            v[j]=v[j-1],opAttr.count();
        opAttr.count();
        v[st]=aux;
    }
}

int binarySearch(int a[], int n, int l, int r, int key){
    Operation opComp = profiler.createOperation("rec-insertion-bin-comp", n);
    int mid=(l+r)/2;
    if(l>r)
        return l;
    opComp.count();
    if(a[mid]==key)
        return mid;
    opComp.count();
    if(a[mid]>key)
        return binarySearch(a, n, l,mid-1,key);
    return binarySearch(a, n,mid+1,r,key);
}

void binaryInsertionRecursive(int a[],int n, int index){
    Operation opComp = profiler.createOperation("rec-insertion-bin-comp", n);
    Operation opAttr = profiler.createOperation("rec-insertion-bin-attr",n);
    if(index==n)
        return;
    int position = binarySearch(a, n, 0,index-1,a[index]);
    opAttr.count();
    int aux=a[index];
    for(int i=index; i>position; i--) {
        opAttr.count();
        a[i] = a[i - 1];
    }
    opAttr.count();
    a[position] = aux;
    binaryInsertionRecursive(a,n,index+1);
}

void demo(){
    int arrHeap[]={3,2,45,4,21,67,43,23,11,12,14};
    int arrQS[]={3,2,45,4,21,67,43,23,11,12,14};
    int arrQSBest[]={3,2,45,4,21,67,43,23,11,12,14};
    int arrRQS[]={3,2,45,4,21,67,43,23,11,12,14};
    int arrBinInsert[]={3,2,45,4,21,67,43,23,11,12,14};
    int arrBinInsertRec[]={3,2,45,4,21,67,43,23,11,12,14};

    int n = sizeof(arrHeap)/sizeof(arrHeap[0]);

    heapSortBU(arrHeap, n,n);
    for(int i=0;i<n;i++)
        cout<<arrHeap[i]<<" ";
    cout<<endl;

    quickSort(arrQS, n, 0, n-1);
    for(int i=0;i<n;i++)
        cout<<arrQS[i]<<" ";
    cout<<endl;

    quickSortBest(arrQSBest, n, 0, n-1);
    for(int i=0;i<n;i++)
        cout<<arrQSBest[i]<<" ";
    cout<<endl;

    randomizedQuickSort(arrRQS, n, 0, n-1);
    for(int i=0;i<n;i++)
        cout<<arrRQS[i]<<" ";
    cout<<endl;

    binaryInsertion(arrBinInsert,n);
    for(int i=0;i<n;i++)
        cout<<arrBinInsert[i]<<" ";
    cout<<endl;

    binaryInsertionRecursive(arrBinInsertRec,n,1);
    for(int i=0;i<n;i++)
        cout<<arrBinInsertRec[i]<<" ";
    cout<<endl;
}

void perf(int order){
    int arrHeap[MAX_SIZE], arrQS[MAX_SIZE],arrRQS[MAX_SIZE], arrBinInsert[MAX_SIZE], arrBinInsertRec[MAX_SIZE];

    if(order==0){
        for(int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
            for (int k = 0; k < NR_TESTS; k++) {
                FillRandomArray(arrHeap, n, 10, 50000, false, order);
                CopyArray(arrQS, arrHeap, n);
                CopyArray(arrRQS, arrHeap, n);
                heapSortBU(arrHeap,n,n);
                quickSort(arrQS, n, 0, n-1);
                randomizedQuickSort(arrRQS, n, 0, n-1);
            }
        }
        profiler.divideValues("heap-sort-comp", NR_TESTS);
        profiler.divideValues("heap-sort-attr", NR_TESTS);
        profiler.addSeries("heap-sort-total","heap-sort-attr","heap-sort-comp");

        profiler.divideValues("quick-sort-comp", NR_TESTS);
        profiler.divideValues("quick-sort-attr", NR_TESTS);
        profiler.addSeries("quick-sort-total","quick-sort-attr","quick-sort-comp");

        profiler.divideValues("random-quick-sort-comp", NR_TESTS);
        profiler.divideValues("random-quick-sort-attr", NR_TESTS);
        profiler.addSeries("random-quick-sort-total","random-quick-sort-attr","random-quick-sort-comp");

        profiler.createGroup("Assignments","heap-sort-attr","quick-sort-attr","random-quick-sort-attr");
        profiler.createGroup("Comparisons","heap-sort-comp","quick-sort-comp","random-quick-sort-comp");
        profiler.createGroup("Total","heap-sort-total","quick-sort-total","random-quick-sort-total");
    }
    else if(order==1){
        for(int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE){
            FillRandomArray(arrHeap, n, 10, 50000, false, 2);
            FillRandomArray(arrQS, n, 10, 50000, false, 1);
            CopyArray(arrRQS, arrQS, n);

            heapSortBU(arrHeap,n,n);
            quickSortBest(arrQS, n, 0, n-1);
            randomizedQuickSort(arrRQS, n, 0, n-1);
        }
        profiler.addSeries("heap-sort-total","heap-sort-attr","heap-sort-comp");
        profiler.addSeries("quick-sort-total","quick-sort-attr","quick-sort-comp");
        profiler.addSeries("random-quick-sort-total","random-quick-sort-attr","random-quick-sort-comp");
        profiler.createGroup("Total","heap-sort-total","quick-sort-total");

        profiler.createGroup("Assignments","heap-sort-attr","quick-sort-attr");
        profiler.createGroup("Comparisons","heap-sort-comp","quick-sort-comp");
        profiler.createGroup("Total","heap-sort-total","quick-sort-total");
        profiler.createGroup("Random Quick Sort","random-quick-sort-total","random-quick-sort-attr","random-quick-sort-comp");
    }
    else{
        for(int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE){
            FillRandomArray(arrHeap, n, 10, 50000, false, order);
            CopyArray(arrQS, arrHeap, n);
            CopyArray(arrRQS, arrHeap, n);
            heapSortBU(arrHeap,n,n);
            quickSort(arrQS, n, 0, n-1);
            randomizedQuickSort(arrRQS, n, 0, n-1);
        }
        profiler.addSeries("heap-sort-total","heap-sort-attr","heap-sort-comp");
        profiler.addSeries("quick-sort-total","quick-sort-attr","quick-sort-comp");
        profiler.addSeries("random-quick-sort-total","random-quick-sort-attr","random-quick-sort-comp");
        profiler.createGroup("Total","heap-sort-total","quick-sort-total","random-quick-sort-total");

        profiler.createGroup("Assignments","heap-sort-attr","quick-sort-attr","random-quick-sort-attr");
        profiler.createGroup("Comparisons","heap-sort-comp","quick-sort-comp","random-quick-sort-comp");
        profiler.createGroup("Total","heap-sort-total","quick-sort-total","random-quick-sort-total");
    }
}

void perfBinaryInsertion(int order){
    int arrBinInsert[MAX_SIZE], arrBinInsertRec[MAX_SIZE];

    if(order==0){
        for(int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
            for (int k = 0; k < NR_TESTS; k++) {
                FillRandomArray(arrBinInsert, n, 10, 50000, false, order);
                CopyArray(arrBinInsertRec, arrBinInsert, n);
                binaryInsertion(arrBinInsert, n);
                binaryInsertionRecursive(arrBinInsertRec, n, 1);
            }

        profiler.divideValues("insertion-bin-comp", NR_TESTS);
        profiler.divideValues("insertion-bin-attr", NR_TESTS);
        profiler.addSeries("insertion-bin-total","insertion-bin-attr","insertion-bin-comp");

        profiler.divideValues("rec-insertion-bin-comp", NR_TESTS);
        profiler.divideValues("rec-insertion-bin-attr", NR_TESTS);
        profiler.addSeries("rec-insertion-bin-total","rec-insertion-bin-attr","rec-insertion-bin-comp");

        profiler.createGroup("Assignments","insertion-bin-attr","rec-insertion-bin-attr");
        profiler.createGroup("Comparisons","insertion-bin-comp","rec-insertion-bin-comp");
        profiler.createGroup("Total","insertion-bin-total","rec-insertion-bin-total");

    }
    else if(order==1){
        for(int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE){
            FillRandomArray(arrBinInsert, n, 10, 50000, false, order);
            CopyArray(arrBinInsertRec, arrBinInsert, n);

            binaryInsertion(arrBinInsert, n);
            binaryInsertionRecursive(arrBinInsertRec, n, 1);
        }

        profiler.addSeries("insertion-bin-total","insertion-bin-attr","insertion-bin-comp");
        profiler.addSeries("rec-insertion-bin-total","rec-insertion-bin-attr","rec-insertion-bin-comp");

        profiler.createGroup("Assignments","insertion-bin-attr","rec-insertion-bin-attr");
        profiler.createGroup("Comparisons","insertion-bin-comp","rec-insertion-bin-comp");
        profiler.createGroup("Total","insertion-bin-total","rec-insertion-bin-total");
    }
    else{
        for(int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE){
            FillRandomArray(arrBinInsert, n, 10, 50000, false, order);
            CopyArray(arrBinInsertRec, arrBinInsert, n);

            binaryInsertion(arrBinInsert, n);
            binaryInsertionRecursive(arrBinInsertRec, n, 1);
        }
        profiler.addSeries("insertion-bin-total","insertion-bin-attr","insertion-bin-comp");
        profiler.addSeries("rec-insertion-bin-total","rec-insertion-bin-attr","rec-insertion-bin-comp");

        profiler.createGroup("Assignments","insertion-bin-attr","rec-insertion-bin-attr");
        profiler.createGroup("Comparisons","insertion-bin-comp","rec-insertion-bin-comp");
        profiler.createGroup("Total","insertion-bin-total","rec-insertion-bin-total");
    }
}

void perfRuntime(){
    int arrRecBinInsert[MAX_SIZE], arrBinInsert[MAX_SIZE];
    for(int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE){
        profiler.startTimer("Binary insertion",n);
        for (int k = 0; k <= 100; k++) {
            FillRandomArray(arrBinInsert, n, 10, 50000, false, 0);
            binaryInsertion(arrBinInsert, n);
        }
        profiler.stopTimer("Binary insertion", n);


        profiler.startTimer("Rec binary insertion",n);
        for (int j = 0; j <= 100; j++) {
            FillRandomArray(arrRecBinInsert, n, 10, 50000, false, 0);
            binaryInsertionRecursive(arrRecBinInsert, n,1);
        }
        profiler.stopTimer("Rec binary insertion", n);
    }
    profiler.createGroup("Runtime iterative vs recursive", "Binary insertion", "Rec binary insertion");
}

void perf_all(){
    profiler.reset("AVERAGE");
    perf(UNSORTED);
    profiler.reset("Best");
    perf(1);
    profiler.reset("Worst");
    perf(2);

    profiler.reset("AVERAGE");
    perfBinaryInsertion(UNSORTED);
    profiler.reset("Best");
    perfBinaryInsertion(1);
    profiler.reset("Worst");
    perfBinaryInsertion(2);

    perfRuntime();
    profiler.showReport();
}

int main()
{
    srand(time(0));
    demo();
    //perf_all();
    return 0;
}