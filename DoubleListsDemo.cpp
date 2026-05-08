#include "containers/doublelinkedlist.h"
#include <fstream>

template <typename Node>
void Print(Node &node, ostream& os){
    os << node << ",";
}

void DoubleLinkedListDemo(){
    cout << endl << "Prueba lista doble enlazada:" << endl;
    DoubleLinkedList<AscendingDoubleLinkedListTrait<TI>> list1;
    list1.push_back(6, 15);
    list1.push_back(2, 25);
    list1.push_back(9, 35);
    list1.push_back(1, 45);
    list1.push_back(7, 55);
    cout << "Prueba del operador << (reutilizado):" << list1 << endl;

    cout << endl<< "Prueba del Destructor (reutilizado): " << endl;
    list1.~DoubleLinkedList();
    cout << "Lista ascendente 1: " << list1 << endl;

    DoubleLinkedList<AscendingDoubleLinkedListTrait<TI>> list2;
    list2.push_back(8, 15);
    list2.push_back(3, 25);
    list2.push_back(7, 35);
    list2.push_back(4, 45);
    list2.push_back(1, 55);

    cout << endl << "Prueba del operador >> (reutilizado): "<<endl;
    ofstream ofs;
    ofs.open("lista2.txt");
    ofs <<  list2 <<   endl;
    ofs.close();

    ifstream file("lista2.txt");
    DoubleLinkedList<AscendingDoubleLinkedListTrait<TI>> list3;
    file >> list3;
    cout << "Lista 3 creada con datos de la lista 2: " << list3 << endl;

    list3.push_back(5, 40);
    cout << endl <<"Prueba Copy Constructor: " << endl;
    DoubleLinkedList<AscendingDoubleLinkedListTrait<TI>> list4(list3);
    cout << "Lista 4: " << list4 << endl;

    using DLI = DoubleLinkedList<AscendingDoubleLinkedListTrait<TI>>::Node;
    
    cout << endl <<"Prueba Forward iterator: " << endl;
    list4.ForEach(Print<DLI>, cout);

    cout << endl <<"Prueba Backward iterator: " << endl;
    list4.ReverseForEach(Print<DLI>, cout);


}

void DoubleListsDemo(){
    DoubleLinkedListDemo();
}