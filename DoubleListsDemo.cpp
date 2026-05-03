#include "containers/doublelinkedlist.h"

void DoubleLinkedListDemo(){
    DoubleLinkedList<AscendingDoubleLinkedListTrait<TI>> list1;
    list1.push_back(6, 15);
    list1.push_back(2, 25);
    list1.push_back(9, 35);
    list1.push_back(1, 45);
    list1.push_back(7, 55);
    cout << "Lista Ascendente: " << list1 << endl;

}

void DoubleListsDemo(){
    DoubleLinkedListDemo();
}