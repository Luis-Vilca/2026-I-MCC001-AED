#include "containers/binarytree.h"
#include <fstream>

using namespace std;

template <typename Iterator>
void PrintTree(Iterator begin, Iterator end, ostream& os = cout)
{
    ForEach(begin, end, [&os](auto& node){ os << node << '\n';});
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
    
    BinaryTree<AscendingBinaryTreeListTrait<TI>> tree1;
    tree1.insert(10,5);
    tree1.insert(20,6);
    tree1.insert(11,7);
    tree1.insert(21,8);
    tree1.insert(8,11);

    // cout << "Prueba iterador forward in order: " << endl;
    // PrintTree(tree1.begin(), tree1.end());

    // cout << "Prueba iterador backward in order: " << endl;
    // PrintTree(tree1.rbegin(), tree1.rend());

    // cout << "Prueba iterador forward preorder: " << endl;
    // PrintTree(tree1.begin_preorder(), tree1.end_preorder());

    // cout << "Prueba iterador backward preorder: " << endl;
    // PrintTree(tree1.rbegin_preorder(), tree1.rend_preorder());

    // cout << "Prueba iterador forward postorder: " << endl;
    // PrintTree(tree1.begin_postorder(), tree1.end_postorder());

    // cout << "Prueba iterador backward postorder: " << endl;
    // PrintTree(tree1.rbegin_postorder(), tree1.rend_postorder());

    // ofstream file("tree.txt");
    // PrintTree(tree1.begin(), tree1.end(), file);
    // file.close();

    // ifstream ifs("tree.txt");
    // BinaryTree<DescendingBinaryTreeListTrait<TI>> tree2;
    // Read(tree2, ifs);
    
    // cout << "Prueba operadores sobrecargados: " << endl;
    // PrintTree(tree2.rbegin(), tree2.rend());


    // auto it = FirstThat(tree2.begin(), tree2.end(), IsEven<BinaryTree<DescendingBinaryTreeListTrait<TI>>::BinaryTreeNode>);
    // if(it != tree2.end()){
    //     cout << "Primer par: "
    //         << *it << endl;
    // }
    // else
    //     cout << "No existe numero par\n";
    
    // //Falta probar el move constructor, el copy constructor, destructor seguro

}
