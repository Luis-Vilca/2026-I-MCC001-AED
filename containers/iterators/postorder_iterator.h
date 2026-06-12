#ifndef __POSTORDER_ITERATOR_H__
#define __POSTORDER_ITERATOR_H__

#include "general_iterator.h"

template <typename Container, typename DerivedIterator, size_t First, size_t Second>
class PostorderIterator : public general_iterator<Container, DerivedIterator> {
    
    using Parent = general_iterator<Container, DerivedIterator>;
    using Parent::Parent;
public:
    DerivedIterator& operator++() {
        using NodePtr = typename Container::NodePtr;
        NodePtr node = this->m_pNode;

        if (!node) 
            return *(this);

        NodePtr parent = node->getParent();
        if (!parent) {
            this->m_pNode = nullptr;
            return *(this);
        }

        if (node == parent->getChild(First) && parent->getChild(Second)) {
            node = parent->getChild(Second);
            while (true) {
                if      (node->getChild(First))  
                    node = node->getChild(First);
                else if (node->getChild(Second)) 
                    node = node->getChild(Second);
                else 
                    break;
            }
        } else {
            node = parent;
        }
        this->m_pNode = node;
        return *(this);
    }
};

#endif // __POSTORDER_ITERATOR_H__