#include "containers/binarytree.h"
#include <fstream>

using namespace std;

void BinaryTreeDemo(){
    BinaryTreeNode<TI> nodo1 = BinaryTreeNode(10, 5);
    BinaryTreeNode<TI>* nodo2 = new BinaryTreeNode<TI>(20, 6);
    nodo1.setChild(0, nodo2);
    
    cout << nodo1 << endl;
    cout << *nodo2 << endl;
    cout << nodo1.getChild(0) <<endl;
    cout << nodo1.getChild(1) <<endl;

    ofstream ofs;
    ofs.open("node1.txt");
    ofs << nodo1 << endl;
    ofs.close();
    
    ifstream file("node1.txt");
    BinaryTreeNode<TI> nodo3 = BinaryTreeNode(0, 0);
    file >> nodo3;
    file.close();

    cout << nodo3 << endl;

}
