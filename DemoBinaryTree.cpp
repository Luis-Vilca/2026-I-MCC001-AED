#include "containers/binarytree.h"
#include <fstream>

using namespace std;

template <typename Node>
void Print(Node &value, ostream& os){
    os << value << ",";
}

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

    BinaryTree<AscendingBinaryTreeListTrait<TI>> tree1;
    tree1.insert(10,5);
    tree1.insert(20,6);
    tree1.insert(11,7);
    tree1.insert(21,8);
    tree1.insert(8,11);

    cout << "Prueba iterador forward in order: " << endl;
    for(auto it = tree1.begin();it != tree1.end();++it)
        cout << *it << endl;

    cout << "Prueba iterador backward in order: " << endl;
    for(auto it = tree1.rbegin(); it != tree1.rend();++it)
        cout << *it << endl;

    cout << "Prueba iterador forward preorder: " << endl;
    for(auto it = tree1.begin_preorder();it != tree1.end_preorder();++it)
        cout << *it << endl;

    cout << "Prueba iterador backward preorder: " << endl;
    for(auto it = tree1.rbegin_preorder();it != tree1.rend_preorder();++it)
        cout << *it << endl;
    
    cout << "Prueba iterador forward postorder: " << endl;
    for(auto it = tree1.begin_postorder();it != tree1.end_postorder();++it)
        cout << *it << endl;

    cout << "Prueba iterador backward postorder: " << endl;
    for(auto it = tree1.rbegin_postorder();it != tree1.rend_postorder();++it)
        cout << *it << endl;

}
