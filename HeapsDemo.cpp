#include <vector>
#include <fstream>
#include "containers/heap.h"

using namespace std;

void HeapDemo(){
    Heap<DescendingHeapTrait<TI>> heap1;
    heap1.insert(5, 20);
    heap1.insert(10, 30);
    heap1.insert(2, 50);
    heap1.insert(7, 11);
    heap1.insert(20, 13);

    Heap<DescendingHeapTrait<TI>> heap2 (heap1);
    cout << heap2 <<endl;

    Heap<DescendingHeapTrait<TI>> heap3 = move(heap2);
    cout << heap3 <<endl;

    ofstream ofs;
    ofs.open("heap1.txt");
    ofs <<  heap1 <<   endl;
    ofs.close();

    // ifstream file("heap1.txt");
    // Heap<AscendingHeapTrait<TI>> heap4;
    // file >> heap4;
    // cout << "Heap 4 creada con datos del heap 1: " << heap4 << endl;
}