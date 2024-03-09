
/*
Avem aici cele 3 (4 cu tot cu inserare binara) sortari pe vectori indexati de la 1 la n.
In functia demo am facut o mica demonstratie ca sortarile functioneaza corect.
In perf am luat un parametru order pt a face toate 3 cazurile cu o singura functie. In caz ca e 0, sunt in cazul average,
unde am luat cate 5 teste pentru fiecare n ca sa fie cat mai fin graficul. Apoi in caz ca e 1, toate sortarile vor folosi vectori
crescatori. In cazul nefavorabil apare o situatie speciala la selection sort unde cazul nu este vectorul sortat descrescator, ci
ceva precum 2 3 4 5 6 7 8 9 1 sau 9 1 2 3 4 5 6 7 8. In cazul vectorului descrescator se fac de 2 ori mai putine atribuiri pentru ca
la fiecare pas cel mai mic si cel mai mare numar ajung la locul lor. In exemplele date, doar cel mai mic, respectiv cel mai mare vor ajunge.

In cazul mediu observam ca sortarea prin inserare binara este cea mai eficienta pt ca face log(i) pasi pt a insera fiecare valoare la locul ei.

In cazul favorabil avem o surpriza deoarece inserarea liniara va gasi mereu pozitia cautata din prima, fiind cu una mai in stanga, in timp ce
prin cautare binara va cauta de la mijloc spre stanga si face log(i) pasi pana o va gasi. De asemenea vedem cum sortarea prin selectie
este ineficienta deoarece cauta mereu liniar in sursa.

In cazul nefavorabil, din nou inserarea binara castiga din acelasi motiv, iar bubble sort e cea mai ineficienta, precum in cazul mediu,
din cauza numarului urias de atribuiri.
 */


#include <iostream>
#include "Profiler.h"
using namespace std;

Profiler p("sorting");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

void insertion(int v[],int n)
{
    Operation opComp = p.createOperation("insertion-comp", n);
    Operation opAttr = p.createOperation("insertion-attr",n);
    for(int i=2;i<=n;i++)
    {
        opAttr.count();
        v[0]=v[i];
        int j=i-1;
        while(v[j]>v[0]) {
            opComp.count();
            opAttr.count();
            v[j + 1] = v[j], j--;
        }
        opComp.count();
        opAttr.count();
        v[j+1]=v[0];
    }
}

void insertion_binary(int v[],int n)
{
    Operation opComp = p.createOperation("insertion_bin-comp", n);
    Operation opAttr = p.createOperation("insertion_bin-attr",n);
    for(int i=2;i<=n;i++)
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

void selection(int v[],int n)
{
    Operation opComp = p.createOperation("selection-comp", n);
    Operation opAttr = p.createOperation("selection-attr",n);
    for(int i=1;i<n;i++)
    {
        int poz=i;
        for(int j=i+1;j<=n;j++) {
            opComp.count();
            if (v[poz] > v[j])
                poz = j;
        }
        if(i!=poz)
        {
            int aux=v[i];
            v[i]=v[poz];
            v[poz]=aux;
            opAttr.count(3);
        }
    }
}
void bubble(int v[],int n)
{
    Operation opComp = p.createOperation("bubble-comp", n);
    Operation opAttr = p.createOperation("bubble-attr",n);
    bool ok;
    do{
        ok=true;
        for(int i=1;i<n;i++) {
            opComp.count();
            if (v[i] > v[i + 1]) {
                int aux = v[i];
                v[i] = v[i + 1];
                v[i + 1] = aux;
                ok = false;
                opAttr.count(3);
            }
        }
        n--;
    }while(ok==0);
}

void printArray(int v[],int n)
{
    for(int i=1;i<=n;i++)
        cout<<v[i]<<" ";
    cout<<endl;
}

void demo()
{
    int v[]={0,1,6,2,67,53,5,4,32,87,86,65,89};
    int v2[]={0,1,6,2,67,53,5,4,32,87,86,65,89};
    int v3[]={0,1,6,2,67,53,5,4,32,87,86,65,89};
    int v4[]={0,1,6,2,67,53,5,4,32,87,86,65,89};
    int n=sizeof(v)/sizeof(v[0])-1;
    insertion(v,n);
    insertion_binary(v2,n);
    selection(v3,n);
    bubble(v4,n);
    printArray(v,n);
    printArray(v2,n);
    printArray(v3,n);
    printArray(v4,n);
}

void perf(int order)
{
    int v1[MAX_SIZE],v2[MAX_SIZE],v3[MAX_SIZE],v4[MAX_SIZE];
    int n;
    if(order!=0) {
        for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
            FillRandomArray(v1, n+1, 10, 50000, 0, order);
            for (int i = 1; i <= n; i++)
                v2[i] = v3[i] = v4[i] = v1[i];
            if (order == 2) {
                FillRandomArray(v3, n, 10, 50000, 0, 1);
                v3[n]=-1;
            }
            bubble(v1, n);
            insertion(v2, n);
            insertion_binary(v4,n);
            selection(v3, n);
        }
        p.addSeries("bubble", "bubble-attr", "bubble-comp");
        p.addSeries("insertion", "insertion-attr", "insertion-comp");
        p.addSeries("insertion_bin", "insertion_bin-attr", "insertion_bin-comp");
        p.addSeries("selection", "selection-attr", "selection-comp");
    }
    else{
        for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
            for(int k=1;k<=NR_TESTS;k++) {
                FillRandomArray(v1, n+1, 10, 50000, 0, order);
                for (int i = 1; i <= n; i++)
                    v2[i] = v3[i] = v4[i] = v1[i];
                bubble(v1, n);
                insertion(v2, n);
                insertion_binary(v4,n);
                selection(v3, n);
            }
        }
        p.divideValues("bubble-attr", NR_TESTS);
        p.divideValues("bubble-comp", NR_TESTS);
        p.addSeries("bubble","bubble-attr","bubble-comp");

        p.divideValues("insertion-attr", NR_TESTS);
        p.divideValues("insertion-comp", NR_TESTS);
        p.addSeries("insertion","insertion-attr","insertion-comp");

        p.divideValues("insertion_bin-attr", NR_TESTS);
        p.divideValues("insertion_bin-comp", NR_TESTS);
        p.addSeries("insertion_bin","insertion_bin-attr","insertion_bin-comp");

        p.divideValues("selection-attr", NR_TESTS);
        p.divideValues("selection-comp", NR_TESTS);
        p.addSeries("selection","selection-attr","selection-comp");
    }


    p.createGroup("Assignments","bubble-attr","insertion-attr","insertion_bin-attr","selection-attr");
    p.createGroup("Comparisons","bubble-comp","insertion-comp", "insertion_bin-comp", "selection-comp");
    p.createGroup("Total","bubble","insertion", "insertion_bin", "selection");
}

void perf_all()
{
    perf(UNSORTED);
    p.reset("Best");
    perf(1);
    p.reset("Worst");
    perf(DESCENDING);
    p.showReport();
}
int main() {
    demo();
    //perf_all();
    return 0;
}
