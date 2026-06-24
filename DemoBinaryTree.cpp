#include <fstream>
#include "containers/binarytree.h"
#include "foreach.h"

using namespace std;

template <typename Iterator>
void Print(Iterator iterator, ostream& os = cout)
{
    ForEach(iterator, [&os](auto& node){ os << node << '\n';});
}

template <typename Tree>
void Read(Tree& tree, istream& is)
{
    typename Tree::Node node(0,0);
    while(is >> node)
        tree.insert(node.getData(), node.getRef());
}

template <typename Node>
bool IsEven(Node& node)
{
    return node.getData() % 2 == 0;
}

void BinaryTreeDemo(){
    
    BinaryTree<AscendingContainerTrait<TI>> tree1;
    tree1.insert(10,5);
    tree1.insert(20,6);
    tree1.insert(11,7);
    tree1.insert(21,8);
    tree1.insert(8,11);

    cout << "Prueba iterador forward in order: " << endl;
    Print(tree1.inorder());

    cout << "Prueba iterador forward in order con for nativo: " << endl;
    for (auto &node : tree1.inorder()){
        cout << node << '\n';
    }

    cout << "Prueba iterador backward in order: " << endl;
    Print(tree1.inorder_reverse());

    cout << "Prueba iterador forward preorder: " << endl;
    Print(tree1.preorder());

    cout << "Prueba iterador backward preorder: " << endl;
    Print(tree1.preorder_reverse());

    cout << "Prueba iterador forward postorder: " << endl;
    Print(tree1.postorder());

    cout << "Prueba iterador backward postorder: " << endl;
    Print(tree1.postorder_reverse());

    ofstream file("tree.txt");
    Print(tree1.inorder(), file);
    file.close();

    ifstream ifs("tree.txt");
    BinaryTree<DescendingContainerTrait<TI>> tree2;
    Read(tree2, ifs);
    
    cout << "Prueba operadores sobrecargados: " << endl;
    Print(tree2.inorder_reverse());

    auto it = FirstThat(tree2.inorder(), IsEven<BinaryTree<DescendingContainerTrait<TI>>::BinaryTreeNode>);
    if(it != tree2.inorder().end()){
        cout << "Primer par: "
            << *it << endl;
    }
    else
        cout << "No existe numero par\n";
    
    //Falta probar el move constructor, el copy constructor, destructor seguro

}
