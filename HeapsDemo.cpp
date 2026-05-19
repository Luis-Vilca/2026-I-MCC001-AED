#include <vector>
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
    cout << heap2 <<endl;
}