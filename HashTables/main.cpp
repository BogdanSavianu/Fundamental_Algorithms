/* In aceasta tema am implementat un tabel de dispersie cu o functie de dispersie patratica.
Am testat performanta functiei de cautare pe mai multi factori de umplere si, dupa cum era de asteptat, aceasta face mai multi pasi
cu cat este mai plina tabela pentru ca, pe parcurs ce am adaugat elementele, a devenit din ce in ce mai greu sa le gasesc un loc liber.
    Am comparat si cele 2 tipuri de cautare, uniforma si neuniforma pe un factor de umplere de 95%. Diferenta este ca cautarea neuniforma va cauta
primele 1500 elemente inserate, in timp ce cea uniforma va lua 1500 elemente la intamplare. Cea neuniforma are defectul ca in functie de
setul de 1500 valori alese, vom avea eforturi diferite direct proportionale cu momentul adaugarii acestora in tabel.
    Apoi dintr-un tabel cu factorul de umplere de 99% am sters elemente pana la 80% si am cautat din nou valori existente si neexistente.
Am observat ca acest tabel, in ciuda noului factor de umplere, se va comporta precum unul cu factor de umplere de 99%. Acest lucru este datorat
faptului ca, in momentul stergerii unei valori, nu am adus inapoi elementele care au creat coliziuni, ci acea pozitie a ramas doar marcata ca DELETED si
cautarea continua in momentul intalnirii unei astfel de celule.
 */


#include <bits/stdc++.h>
#include <cstring>
#include "Profiler.h"

using namespace std;
#define N 9973
#define MAX_SIZE 9973
#define NR_TESTS 5
#define DELETED (-1234)
Profiler profiler("hashTables");

typedef struct {
    int id;
    char name[30];
} Entry;

long h(int key, int i) {
    int k1 = 53, k2 = 13, k3 = 13;
    return (key + k1 * i * i + k2 * i + k3) % N;
}

void insert(Entry table[], Entry entry) {
    int i = 0;
    do {
        int j = h(entry.id, i);
        if (table[j].id == -1 || table[j].id == DELETED) {
            table[j].id = entry.id;
            strcpy(table[j].name, entry.name);
            return;
        } else i++;
    } while (i < N);
    cout << "table overflow" << endl;
}

void insertInHash(Entry table[], int v[], int n) {
    for (int i = 0; i < n; i++)
        insert(table, {v[i], "dndn"});
}

int search(Entry table[], int key, int &op) {
    int i = 0, j;
    do {
        j = h(key, i);
        op++;
        if (table[j].id == key)
            return j;
        i++;
    } while (table[j].id != -1 && i < N);
    return -1;
}

int deleteHash(Entry table[], int key) {
    int dushfdhgf;
    int positionToBeDeleted = search(table, key, dushfdhgf);
    if (positionToBeDeleted != -1) {
        table[positionToBeDeleted].id = DELETED;
        strcpy(table[positionToBeDeleted].name, "DELETED");
        return positionToBeDeleted;
    }
    return -1;
}

void shuffleArray(int v[], int n) {
    unsigned seed = 0;
    shuffle(v, v + n, default_random_engine(seed));
}


void demo() {
    int n = 10, fdfshfdv;
    Entry table[N];
    for (int i = 0; i < N; i++)
        table[i].id = -1, strcpy(table[i].name,"empty");
    insert(table, {1, "mama"});
    insert(table, {11, "George"});
    insert(table, {13, "Daniel"});
    insert(table, {18, "Manuel"});
    insert(table, {18, "David"});
    insert(table, {21, "Jack"});
    insert(table, {17, "Jim"});
    insert(table, {29, "Michael"});
    insert(table, {11, "Eric"});
    insert(table, {11, "Ronaldo"});

    for (int i = 0; i < N; i++)
        cout << i << ": (" << table[i].id << ", " << table[i].name << ")" << endl;
    cout << endl;
    cout <<"The entry with id "<<29<<" has been found at index "<<search(table, 29, fdfshfdv)<<endl;
    cout<<"The entry with id "<<29<<" will be deleted";
    deleteHash(table, 29);
    cout << endl << endl;
    for (int i = 0; i < N; i++)
        cout << i << ": (" << table[i].id << ", " << table[i].name << ")" << endl;
}

void nonUniform(){
    Entry table[MAX_SIZE];
    double alfa = 0.95;
    int opFound,opNotFound,found[1500],notFound[1500];

    double avgFoundEffort = 0, avgNotFoundEffort = 0;
    int maxFoundEffort = 0, maxNotFoundEffort = 0;

    for(int i=0; i<NR_TESTS; i++){
        for (int j = 0; j < N; j++)
            table[j].id = -1;
        int nrElements = (int) (alfa * N);
        int aux[nrElements];
        FillRandomArray(aux, nrElements, 10, 50000, false, 0);
        insertInHash(table, aux, nrElements);
        CopyArray(found, aux, 1500);
        FillRandomArray(notFound, 1500, 501000, 503000, false, 0);

        for (int l = 0; l < 1500; l++) {
            opFound = 0;
            int foundIndex = search(table, found[l], opFound);
            if (foundIndex != -1) {
                if (maxFoundEffort < opFound)
                    maxFoundEffort = opFound;
                avgFoundEffort += (double) opFound;
            }
        }
        for (int l = 0; l < 1500; l++) {
            opNotFound = 0;
            search(table, notFound[l], opNotFound);
            if (maxNotFoundEffort < opNotFound)
                maxNotFoundEffort = opNotFound;
            avgNotFoundEffort += (double) opNotFound;
        }
    }
    avgFoundEffort /= 1500.0;
    avgFoundEffort /= 5.0;
    avgNotFoundEffort /= 1500.0;
    avgNotFoundEffort /= 5.0;

    cout << "For alfa with = " << alfa<< " with non-uniform search we have:" << endl;
    cout << "Average effort for found numbers: " << avgFoundEffort << endl;
    cout << "Average effort for non-found number: " << avgNotFoundEffort << endl;
    cout << "Max effort for found numbers: " << maxFoundEffort << endl;
    cout << "Max effort for non-found numbers: " << maxNotFoundEffort;
}

void perf() {
    double alfa[] = {0.8, 0.85, 0.9, 0.95, 0.99};
    int opFound, opNotFound;
    int found[1500];
    int notFound[1500];
    double avgFoundEffort = 0, avgNotFoundEffort = 0;
    int maxFoundEffort = 0, maxNotFoundEffort = 0;
    int length = sizeof(alfa) / sizeof(alfa[0]);
    for (int i = 0; i < length; i++) {
        avgFoundEffort = 0, avgNotFoundEffort = 0;
        maxFoundEffort = 0, maxNotFoundEffort = 0;

        for (int k = 0; k < NR_TESTS; k++) {
            Entry table[MAX_SIZE];
            opFound = 0, opNotFound = 0;
            for (int j = 0; j < N; j++)
                table[j].id = -1;
            int nrElements = (int) (alfa[i] * N);
            int aux[nrElements];
            FillRandomArray(aux, nrElements, 10, 50000, false, 0);
            insertInHash(table, aux, nrElements);
            shuffleArray(aux, nrElements);
            CopyArray(found, aux, 1500);
            FillRandomArray(notFound, 1500, 501000, 503000, false, 0);

            for (int l = 0; l < 1500; l++) {
                opFound = 0;
                int foundIndex = search(table, found[l], opFound);
                if (foundIndex != -1) {
                    if (maxFoundEffort < opFound)
                        maxFoundEffort = opFound;
                    avgFoundEffort += (double) opFound;
                }
            }
            for (int l = 0; l < 1500; l++) {
                opNotFound = 0;
                search(table, notFound[l], opNotFound);
                if (maxNotFoundEffort < opNotFound)
                    maxNotFoundEffort = opNotFound;
                avgNotFoundEffort += (double) opNotFound;
            }
        }
        avgFoundEffort /= 1500.0;
        avgFoundEffort /= 5.0;
        avgNotFoundEffort /= 1500.0;
        avgNotFoundEffort /= 5.0;

        cout << "For alfa = " << alfa[i] << " we have:" << endl;
        cout << "Average effort for found numbers: " << avgFoundEffort << endl;
        cout << "Average effort for non-found number: " << avgNotFoundEffort << endl;
        cout << "Max effort for found numbers: " << maxFoundEffort << endl;
        cout << "Max effort for non-found numbers: " << maxNotFoundEffort << endl << endl << endl;
    }
    double alfaDelete = 0.99;
    int nrElements = (int) (alfaDelete * N);
    Entry table[MAX_SIZE];
    opFound = 0, opNotFound = 0;
    for (int j = 0; j < N; j++)
        table[j].id = -1;
    int aux[nrElements];
    FillRandomArray(aux, nrElements, 10, 50000, false, 0);
    insertInHash(table, aux, nrElements);
    int copyNrElements = nrElements;

    avgFoundEffort = 0, avgNotFoundEffort = 0;
    maxFoundEffort = 0, maxNotFoundEffort = 0;
    shuffleArray(aux, nrElements);
    while (alfaDelete > 0.8) {
            int position = deleteHash(table, aux[rand()%nrElements]);
            if (position != -1) {
                copyNrElements--;
                alfaDelete = (double) copyNrElements / N;
            }
    }

    CopyArray(found, aux, 1500);
    FillRandomArray(notFound, 1500, 501000, 503000, false, 0);

    int nrFound = 0;
    for (int l = 0; l < 1500; l++) {
        opFound = 0;
        int foundIndex = search(table, found[l], opFound);
        if (foundIndex != -1) {
            if(foundIndex != DELETED) {
                if (maxFoundEffort < opFound)
                    maxFoundEffort = opFound;
                avgFoundEffort += (double) opFound;
                nrFound++;
            }
        }
    }
    for (int l = 0; l < 1500; l++) {
        opNotFound = 0;
        search(table, notFound[l], opNotFound);
        if (maxNotFoundEffort < opNotFound)
            maxNotFoundEffort = opNotFound;
        avgNotFoundEffort += (double) opNotFound;
    }
    avgFoundEffort /= nrFound;
    avgNotFoundEffort /= 1500.0;

    cout << "AFTER DELETION DOWN TO ALFA=0.8:" << endl;
    cout << "Average effort for found numbers: " << avgFoundEffort << endl;
    cout << "Average effort for non-found number: " << avgNotFoundEffort << endl;
    cout << "Max effort for found numbers: " << maxFoundEffort << endl;
    cout << "Max effort for non-found numbers: " << maxNotFoundEffort << endl;
}

int main() {
    //demo();
    //nonUniform();
    perf();
    return 0;
}
