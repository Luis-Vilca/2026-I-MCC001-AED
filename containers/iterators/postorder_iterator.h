#ifndef __POSTORDER_ITERATOR_H__
#define __POSTORDER_ITERATOR_H__

#include "general_iterator.h"

template <typename Container, typename DerivedIterator, size_t First, size_t Second>
class PostorderIterator : public general_iterator<Container, DerivedIterator> {
    
    using Parent = general_iterator<Container, DerivedIterator>;
    using Parent::Parent;
    using NodePtr = typename Container::NodePtr;
    
public:

    static NodePtr first(NodePtr root){
        while(root) {
            if(root->getChild(First))
                root = root->getChild(First);
            else if(root->getChild(Second))
                root = root->getChild(Second);
            else
                break;
        }

        return root;
    }

    DerivedIterator& operator++() {
        NodePtr node = this->m_pNode;

        if (!node) 
            return this->self();

        NodePtr parent = node->getParent();
        if (!parent) {
            this->m_pNode = nullptr;
            return this->self();
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
        return this->self();
    }
};

#endif // __POSTORDER_ITERATOR_H__