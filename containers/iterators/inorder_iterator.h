#ifndef __INORDER_ITERATOR_H__
#define __INORDER_ITERATOR_H__

#include "general_iterator.h"
using namespace std;

template <typename Container, typename DerivedIterator, size_t First, size_t Second>
class InorderIterator : public general_iterator<Container, DerivedIterator> {

    using Parent = general_iterator<Container, DerivedIterator>;
    using Parent::Parent;
public:
    // TODO: Completar el operator++
    DerivedIterator& operator++(){
        
        using NodePtr = typename Container::NodePtr;
        NodePtr node = this->m_pNode;

        if (!node)
            return *this;
        if (node->getChild(Second)) {
            node = node->getChild(Second);
            while (node->getChild(First))
                node = node->getChild(First);
        }
        else {
            NodePtr parent = node->getParent();
            while (parent && node == parent->getChild(Second))
            {
                node = parent;
                parent = parent->getParent();
            }
            node = parent;
        }
        this->m_pNode = node;
        return *this;
    }
};

#endif //__INORDER_ITERATOR_H__