#include "containers/circulardoublelinkedlist.h"

template <typename Node>
void Print(Node &node, ostream& os){
    os << node << ",";
}

void CircularDoubleLinkedListDemo(){
    cout << endl << "Prueba lista circular doble enlazada:" << endl;
    CircularDoubleLinkedList<AscendingCircularDoubleLinkedListTrait<TI>> list1;
    list1.insert(6, 15);
    list1.insert(2, 25);
    list1.insert(9, 35);
    list1.insert(1, 45);
    list1.insert(7, 55);
    //cout << "Prueba del operador << (reutilizado):" << list1 << endl;
    using CDLI = CircularDoubleLinkedList<AscendingCircularDoubleLinkedListTrait<TI>>::Node;
    list1.ForEach(Print<CDLI>, cout);
    cout << endl;

    CircularDoubleLinkedList<DescendingCircularDoubleLinkedListTrait<TI>> list2;
    list2.insert(6, 15);
    list2.insert(2, 25);
    list2.insert(9, 35);
    list2.insert(1, 45);
    list2.insert(7, 55);
    cout << endl << "Prueba del operador << (reutilizado):" << list2 << endl;
}

void CircularDoubleListsDemo(){
    CircularDoubleLinkedListDemo();
}