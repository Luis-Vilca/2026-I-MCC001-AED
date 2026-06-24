#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__
 
#include "binarytree.h"
 
template <typename Traits>
class AVLTree : public BinaryTree<Traits> {
public:

    using Base       = BinaryTree<Traits>;
    using value_type = typename Base::value_type;
    using MySelf     = AVLTree<Traits>;
 
    class AVLNode : public Base::BinaryTreeNode {
    public:
        using ParentNode = typename Base::BinaryTreeNode;
        using NodePtr    = AVLNode*;
 
        size_t m_height = 1;

        AVLNode(const value_type& data, const Ref& ref,
                NodePtr left  = nullptr, NodePtr right = nullptr)
                : ParentNode(data, ref, left, right)
        {}
    };

    using NodePtr    = AVLNode*;
 
    //Constructor y destructor se heredan de BinaryTree

    void insert(const value_type& value, Ref ref) {
        scoped_lock<mutex> lock(this -> m_mtx);
        NodePtr inserted = static_cast<NodePtr>(BinaryTree<Traits>::internal_insert(this->m_pRoot, value, ref));
        after_insert(inserted);
    }

protected:
    NodePtr createNode(const value_type& value, Ref ref) override {
        return new AVLNode(value, ref);
    }

private:

    static size_t height(NodePtr n) {
        return n ? n->m_height : 0;
    }
 
    static void update_height(NodePtr pNode) {
        if (pNode)
            pNode->m_height = 1 + max(height(NodePtr(pNode->getChild(0))),
                                  height(NodePtr(pNode->getChild(1))));
    }

    static TI balance_factor(NodePtr pNode) {
        if (!pNode) return 0;
        return height(NodePtr(pNode->getChild(0))) - height(NodePtr(pNode->getChild(1)));
    }
 
    NodePtr rotate_right(NodePtr y) {
        NodePtr x   = NodePtr(y->getChild(0));
        NodePtr T2 = NodePtr(x->getChild(1));
 
        // Giro
        x->setChild(1, y);
        y->setChild(0, T2);
 
        x->setParent(y->getParent());
        y->setParent(x);
        if (T2) T2->setParent(y);

        update_height(y);
        update_height(x);
        return x;   // nueva raíz del subárbol
    }
 
    NodePtr rotate_left(NodePtr x) {
        NodePtr y  = NodePtr(x->getChild(1));
        NodePtr T2 = NodePtr(y->getChild(0));
 
        y->setChild(0, x);
        x->setChild(1, T2);

        y->setParent(x->getParent());
        x->setParent(y);
        if (T2) T2->setParent(x);
 
        update_height(x);
        update_height(y);
        return y;  
    }
 
    NodePtr rebalance(NodePtr node) {
        update_height(node);
        TI bf = balance_factor(node);
 
        if (bf > 1 && balance_factor(NodePtr(node->getChild(0))) >= 0)
            return rotate_right(node);

        if (bf < -1 && balance_factor(NodePtr(node->getChild(1))) <= 0)
            return rotate_left(node);

        if (bf > 1 && balance_factor(NodePtr(node->getChild(0))) < 0) {
            node->setChild(0, rotate_left(NodePtr(node->getChild(0))));
            return rotate_right(node);
        }

        if (bf < -1 && balance_factor(NodePtr(node->getChild(1))) > 0) {
            node->setChild(1, rotate_right(NodePtr(node->getChild(1))));
            return rotate_left(node);
        }
        return node;  
    }

    void after_insert(NodePtr node) {
        while (node) {
            NodePtr parent  = NodePtr(node->getParent());
            NodePtr newNode = rebalance(node);

            if (newNode != node) {
                if (parent) {
                    if (parent->getChild(0) == node)
                        parent->setChild(0, newNode);
                    else
                        parent->setChild(1, newNode);
                } else {
                    this->m_pRoot = newNode;
                }
            }
            node = parent;
        }
    }
 
};
#endif // __AVL_TREE_H__