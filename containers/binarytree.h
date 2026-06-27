#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__
#include <iostream>
#include <cstddef>   // size_t
#include <string>
#include <sstream>
#include <mutex>
#include "iterators/general_iterator.h"
#include "iterators/inorder_iterator.h"
#include "iterators/postorder_iterator.h"
#include "iterators/preorder_iterator.h"
#include "iterators/TreeIterators.h"
#include "basetrait.h"
#include "../foreach.h"
#include "../types.h"

//Comentarios de mejora:

//Sacar los prototipos de la clase nodo fuera
//Colocar todo como Node en AVL y ya no es necesario agregar el AVLNode

template <typename Container>
class BinaryTreeForwardInorderIterator: public InorderIterator<Container, 
                                               BinaryTreeForwardInorderIterator<Container>, 0, 1> {
    using Parent = InorderIterator<Container, BinaryTreeForwardInorderIterator<Container>, 0, 1>;
    using Parent::Parent;
};

template <typename Container>
class BinaryTreeBackwardInorderIterator: public InorderIterator<Container,
                                                BinaryTreeBackwardInorderIterator<Container>, 1, 0> {
    using Parent = InorderIterator<Container, BinaryTreeBackwardInorderIterator<Container>, 1, 0>;
    using Parent::Parent;
};

template <typename Container>
class BinaryTreeForwardPreorderIterator: public PreorderIterator<Container,
                                                BinaryTreeForwardPreorderIterator<Container>, 0, 1> {
    using Parent = PreorderIterator<Container, BinaryTreeForwardPreorderIterator<Container>, 0, 1>;
    using Parent::Parent;
};

template <typename Container>
class BinaryTreeBackwardPreorderIterator: public PreorderIterator<Container,
                                                 BinaryTreeBackwardPreorderIterator<Container>, 1, 0> {
    using Parent = PreorderIterator<Container, BinaryTreeBackwardPreorderIterator<Container>, 1, 0>;
    using Parent::Parent;
};

template <typename Container>
class BinaryTreeForwardPostorderIterator: public PostorderIterator<Container,
                                                 BinaryTreeForwardPostorderIterator<Container>, 0, 1> {
    using Parent = PostorderIterator<Container, BinaryTreeForwardPostorderIterator<Container>, 0, 1>;
    using Parent::Parent;
};

template <typename Container>
class BinaryTreeBackwardPostorderIterator: public PostorderIterator<Container,
                                                  BinaryTreeBackwardPostorderIterator<Container>, 1, 0> {
    using Parent = PostorderIterator<Container, BinaryTreeBackwardPostorderIterator<Container>, 1, 0>;
    using Parent::Parent;
};

template <typename Traits>
class BinaryTree{
public:
    class BinaryTreeNode;
    using value_type = typename Traits::value_type;
    using Node       = BinaryTreeNode;
    using NodePtr    = Node*;
    using Comp       = typename Traits::Comp;
    using MySelf     = BinaryTree<Traits>;

    using forward_inorder_iterator    = BinaryTreeForwardInorderIterator<MySelf>;
    using backward_inorder_iterator   = BinaryTreeBackwardInorderIterator<MySelf>;
    using forward_preorder_iterator   = BinaryTreeForwardPreorderIterator<MySelf>;
    using backward_preorder_iterator  = BinaryTreeBackwardPreorderIterator<MySelf>;
    using forward_postorder_iterator  = BinaryTreeForwardPostorderIterator<MySelf>;
    using backward_postorder_iterator = BinaryTreeBackwardPostorderIterator<MySelf>;

protected:
    NodePtr m_pRoot = nullptr;
    Comp m_comp;
    mutex m_mtx;
    mutex m_mtx2;
public:
    
    //Node dentro del contenedor
    class BinaryTreeNode{
    public:
        using value_type = typename Traits::value_type;
        using Node       = BinaryTreeNode;
        using NodePtr    = Node*;
    protected:
        value_type m_data;
        Ref        m_ref;
        NodePtr    m_pChild[2] = {nullptr, nullptr};
        NodePtr    m_pParent;   
        NodePtr    m_right;
    public:
        BinaryTreeNode(const value_type& data, const Ref& ref, 
            NodePtr left = nullptr, NodePtr right = nullptr)
            : m_data(data), m_ref(ref), m_pParent(nullptr), m_right(nullptr)
        {
            m_pChild[0] = left;
            m_pChild[1] = right;
            if (left)
                left->m_pParent = this;
            if (right)
                right->m_pParent = this;
        }
        // copy constructor ... tiene error
        BinaryTreeNode(const BinaryTreeNode& other)
            : m_data(other.m_data), m_ref(other.m_ref)
        {
            m_pChild[0] = other.m_pChild[0] ? new Node(*other.m_pChild[0]) : nullptr;
            m_pChild[1] = other.m_pChild[1] ? new Node(*other.m_pChild[1]) : nullptr;
            updateChildrenParent();
        }
        // Corregir con exchange
        BinaryTreeNode(BinaryTreeNode&& other) noexcept
            : m_data(move(other.m_data)), m_ref(move(other.m_ref))
        {
            m_pChild[0] = exchange(other.m_pChild[0], nullptr);
            m_pChild[1] = exchange(other.m_pChild[1], nullptr);
            updateChildrenParent();
        }
        ~BinaryTreeNode() {
            delete m_pChild[0];
            delete m_pChild[1];
        };

        value_type      getData() const { return m_data; }
        value_type&     getDataRef()    { return m_data; }
        void            setData(value_type data) { m_data = data; }
        Ref             getRef() const  { return m_ref; }
        Ref&            getRefRef()     { return m_ref; }
        void            setRef(Ref ref) { m_ref = ref; }

        NodePtr         getChild(size_t pos) const { return m_pChild[pos]; }
        NodePtr&        getChildRef(size_t pos)    { return m_pChild[pos]; }
        void            setChild(size_t pos, NodePtr pChild) {  m_pChild[pos] = pChild; 
                                                                if (pChild)
                                                                    pChild->m_pParent = this;
                                                            }
        void            setParent(NodePtr pParent) { m_pParent = pParent; }
        NodePtr         getParent() const { return m_pParent; }
        
        string to_string() const {
            stringstream ss;
            ss << "Node(data: " << m_data << ", ref: " << m_ref << ")";
            return ss.str();
        }
        
        void updateChildrenParent() {
            if (m_pChild[0])
                m_pChild[0]->m_pParent = this;

            if (m_pChild[1])
                m_pChild[1]->m_pParent = this;
        }
        // Cuidado: en el disco hay posiciones dentro del archivo,
        //          en memoria hay punteros
        friend ostream& operator<<(ostream& os, 
            const BinaryTreeNode& node) {
                os << node.getData() << ' ' << node.getRef();
            return os;
        }
        // Cuidado: en el disco hay posiciones dentro del archivo,
        //          en memoria hay punteros
        friend istream& operator>>(istream& is, 
            BinaryTreeNode& node) {
            is >> node.m_data >> node.m_ref;
            return is;
        }
    };

    BinaryTree() {}
    BinaryTree(const BinaryTree &other)
        : m_comp(other.m_comp) { // Copy constructor

        scoped_lock<mutex> lock(m_mtx2);
        m_pRoot = other.m_pRoot ? new Node(*other.m_pRoot) : nullptr;
    };
    BinaryTree(BinaryTree &&other) { // Move constructor
        
        scoped_lock<mutex> lock(m_mtx);
        m_comp  = move(other.m_comp);
        m_pRoot = exchange(other.m_pRoot, nullptr);
    };

    ~BinaryTree() {
        scoped_lock<mutex> lock(m_mtx);
        delete m_pRoot;
    };

    void insert(const value_type &value, Ref ref){
        scoped_lock<mutex> lock(m_mtx);
        internal_insert(m_pRoot, value, ref);
    }
    
    TreeIterators<forward_inorder_iterator> inorder() {
        return { forward_inorder_iterator(this, forward_inorder_iterator::first(m_pRoot)),
                 forward_inorder_iterator(this, nullptr),
                 &m_mtx};
    }
 
    TreeIterators<backward_inorder_iterator> inorder_reverse() {
        return { backward_inorder_iterator(this, backward_inorder_iterator::first(m_pRoot)),
                 backward_inorder_iterator(this, nullptr),
                 &m_mtx};
    }
 
    TreeIterators<forward_preorder_iterator> preorder() {
        return { forward_preorder_iterator(this, forward_preorder_iterator::first(m_pRoot)),
                 forward_preorder_iterator(this, nullptr),
                 &m_mtx };
    }
 
    TreeIterators<backward_preorder_iterator> preorder_reverse() {
        return { backward_preorder_iterator(this, backward_preorder_iterator::first(m_pRoot)),
                 backward_preorder_iterator(this, nullptr),
                 &m_mtx };
    }
 
    TreeIterators<forward_postorder_iterator> postorder() {
        return { forward_postorder_iterator(this, forward_postorder_iterator::first(m_pRoot)),
                 forward_postorder_iterator(this, nullptr),
                 &m_mtx };
    }
 
    TreeIterators<backward_postorder_iterator> postorder_reverse() {
        return { backward_postorder_iterator(this, backward_postorder_iterator::first(m_pRoot)),
                 backward_postorder_iterator(this, nullptr),
                 &m_mtx };
    }

protected:

    virtual NodePtr createNode(const value_type& value, Ref ref) {
        return new Node(value, ref);
    }

    NodePtr internal_insert(NodePtr &pNode, const value_type &value, Ref ref, NodePtr parent = nullptr){
        
        if( !pNode ){
            pNode = createNode(value, ref);
            pNode->setParent(parent);
            return pNode;
        }
        size_t pos = !m_comp(value, pNode->getDataRef());
        NodePtr inserted = internal_insert(pNode->getChildRef(pos), value, ref, pNode);
        return  inserted; 
    }
};

#endif // __BINARY_TREE_H__