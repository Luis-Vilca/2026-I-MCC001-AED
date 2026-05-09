#include "containers/circularlinkedlist.h"

template <typename Node>
void Print(Node &node, ostream& os){
    os << node << ",";
}

void CircularLinkedListDemo(){
    cout << endl << "Prueba lista circular enlazada:" << endl;
    CircularLinkedList<AscendingCircularLinkedListTrait<TI>> list1;
    list1.insert(6, 15);
    list1.insert(2, 25);
    list1.insert(9, 35);
    list1.insert(1, 45);
    list1.insert(7, 55);
    using CLI = CircularLinkedList<AscendingCircularLinkedListTrait<TI>>::Node;
    list1.ForEach(Print<CLI>, cout);
    cout << endl;

    CircularLinkedList<DescendingCircularLinkedListTrait<TI>> list2;
    list2.insert(6, 15);
    list2.insert(2, 25);
    list2.insert(9, 35);
    list2.insert(1, 45);
    list2.insert(7, 55);
    cout << endl << "Prueba del operador << (reutilizado):" << list2 << endl;
}

void CircularListsDemo(){
    CircularLinkedListDemo();
}