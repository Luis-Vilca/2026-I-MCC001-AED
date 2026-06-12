#include "containers/avltree.h"
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

void AVLTreeDemo(){
    
    AVLTree<AscendingAVLTreeTrait<int>> avl1;
    avl1.insert(10, 11);
    avl1.insert(20, 12);
    avl1.insert(5,  13);
    avl1.insert(3,  14);
    avl1.insert(1,  15);

    // cout << "Prueba iterador forward in order: " << endl;
    // PrintTree(avl1.begin(), avl1.end());

}
