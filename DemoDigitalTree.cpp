#include <iostream>
#include "containers/digitaltree.h"
#include "types.h"

using namespace std;

void DemoDigitalTree()
{
    cout << "==========================================" << endl;
    cout << "        DEMO DIGITAL TREE (TRIE)" << endl;
    cout << "==========================================" << endl;

    DigitalTree<TI> tree;

    cout << "\nInsertando palabras...\n";
    tree.Insert("CASA", 10);
    tree.Insert("CASCO", 20);
    tree.Insert("CASO", 30);
    tree.Insert("PERRO", 40);
    tree.Insert("GATO", 50);
    cout << "Cantidad de elementos: " << tree.Size()<< endl;

    cout << "\nBuscando palabras...\n";
    auto node = tree.Search("CASA");
    if(node)
        cout << "CASA encontrada -> "<< node->Value() << endl;
    else
        cout << "CASA no encontrada\n";

    node = tree.Search("CASCO");
    if(node)
        cout << "CASCO encontrada -> "<< node->Value() << endl;
    else
        cout << "CASCO no encontrada\n";

    node = tree.Search("CAS");
    if(node)
        cout << "CAS encontrada\n";
    else
        cout << "CAS no es una palabra almacenada\n";
    node = tree.Search("PERA");

    if(node)
        cout << "PERA encontrada\n";
    else
        cout << "PERA no encontrada\n";


    cout << "\nRecorrido DFS:\n";
    for(auto& node : tree.forward())
        cout << node.Key() << " -> " << node.Value() << endl;
    
    cout << "\nEliminando CASA...\n";
    tree.Remove("CASA");
    if(tree.Search("CASA"))
        cout << "CASA sigue existiendo\n";
    else
        cout << "CASA eliminada correctamente\n";

    cout << "\nRecorrido DFS:\n";
    for(auto& node : tree.forward())
        cout << node.Key() << " -> " << node.Value() << endl;

    cout << boolalpha;
    cout << "Empieza con CAS: "  << tree.StartsWith("CAS")  << endl;    // true
    cout << "Empieza con PE: "   << tree.StartsWith("PE")   << endl;    // true
    cout << "Empieza con PER: "  << tree.StartsWith("PER")  << endl;    // true
    cout << "Empieza con PERA: " << tree.StartsWith("PERA") << endl;    // false
    cout << "Empieza con XYZ: "  << tree.StartsWith("XYZ")  << endl;    // false

    cout << "\nLimpiando arbol...\n";
    tree.Clear();
    cout << "Elementos restantes: " << tree.Size() << endl;
    cout << endl;
}