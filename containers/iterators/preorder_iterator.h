#ifndef __PREORDER_ITERATOR_H__
#define __PREORDER_ITERATOR_H__

#include "general_iterator.h"

template <typename Container, typename DerivedIterator, size_t First, size_t Second>
class PreorderIterator : public general_iterator<Container, DerivedIterator> {
    
    using Parent = general_iterator<Container, DerivedIterator>;
    using Parent::Parent;
    using NodePtr = typename Container::NodePtr;
    
public:

    static NodePtr first(NodePtr root){
        while(root) {
            if(root->getChild(Second))
                root = root->getChild(Second);
            else if(root->getChild(First))
                root = root->getChild(First);
            else
                break;
        }
        return root;
    }

    DerivedIterator& operator++() {

        NodePtr node = this->m_pNode;

        if (!node) 
            return this->self();

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
        return this->self();
    }
};

#endif // __PREORDER_ITERATOR_H__