#ifndef __PREORDER_ITERATOR_H__
#define __PREORDER_ITERATOR_H__

#include "general_iterator.h"

template <typename Container, typename DerivedIterator, size_t First, size_t Second>
class PreorderIterator : public general_iterator<Container, DerivedIterator> {
    
    using Parent = general_iterator<Container, DerivedIterator>;
    using Parent::Parent;
public:
    DerivedIterator& operator++() {
        using NodePtr = typename Container::NodePtr;
        NodePtr node = this->m_pNode;

        if (!node) 
            return *(this);

        if (node->getChild(First))
            node = node->getChild(First);

        else if (node->getChild(Second))
            node = node->getChild(Second);

        else {
            NodePtr parent = node->getParent();
            while (parent) {
                if (node == parent->getChild(First) && parent->getChild(Second)) {
                    node = parent->getChild(Second);
                    break;
                }
                node   = parent;
                parent = parent->getParent();
            }
            if (!parent) 
                node = nullptr;
        }
        this->m_pNode = node;
        return *(this);
    }
};

#endif // __PREORDER_ITERATOR_H__