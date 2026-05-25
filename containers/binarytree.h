#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__
#include <iostream>
#include <cstddef>   // size_t
#include <string>
#include <sstream>
#include "general_iterator.h"
#include "basetrait.h"
#include "../types.h"

template <typename Container>
class BinaryTreeForwardInorderIterator : public general_iterator<Container, 
                                         BinaryTreeForwardInorderIterator<Container>>{
    using MySelf = BinaryTreeForwardInorderIterator<Container>;
    using Parent = general_iterator<Container, MySelf>;
    using Parent::Parent;
public:
    // TODO: Completar el operator++
    MySelf& operator++(){
        // this->m_pNode = this->m_pNode->getNext();
        return *this;
    }
};

template <typename Container>
class BinaryTreeBackwardInorderIterator : public general_iterator<Container, 
                                         BinaryTreeBackwardInorderIterator<Container>>{
    using MySelf = BinaryTreeBackwardInorderIterator<Container>;
    using Parent = general_iterator<Container, MySelf>;
    using Parent::Parent;
public:
    // TODO: Completar el operator++
    MySelf& operator++(){
        // this->m_pNode = this->m_pNode->getNext();
        return *this;
    }
};

template <typename T>
class BinaryTreeNode{
public:
    using value_type = T;
    using Node       = BinaryTreeNode<T>;
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
        os << node.m_data << ' ' << node.m_ref;
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

template <typename T>
struct BaseBinaryTreeListTrait : public BaseContainerTrait<T, BinaryTreeNode<T>>{

};

template <typename T>
struct AscendingBinaryTreeListTrait : public BaseBinaryTreeListTrait<T>,
                                      public AscendingTrait<T>
{
};

template <typename T>
struct DescendingBinaryTreeListTrait : public BaseBinaryTreeListTrait<T>,
                                       public DescendingTrait<T>
{
};

template <typename Traits>
class BinaryTree{
public:
    using value_type = typename Traits::value_type;
    using Node       = typename Traits::Node;
    using NodePtr    = typename Traits::Node*;
    using Comp       = typename Traits::Comp;
    using MySelf     = BinaryTree<Traits>;

    using forward_inorder_iterator  = BinaryTreeForwardInorderIterator<MySelf>;
    using backward_inorder_iterator = BinaryTreeBackwardInorderIterator<MySelf>;

protected:
    NodePtr m_pRoot = nullptr;
    Comp m_comp;
public:
    BinaryTree() {}
    BinaryTree(const BinaryTree &other)
        : m_comp(other.m_comp) { // Copy constructor

        m_pRoot = other.m_pRoot ? new Node(*other.m_pRoot) : nullptr;
    };
    BinaryTree(BinaryTree &&other)
        : m_comp(other.m_comp){ // Move constructor
        
        m_pRoot = other.m_pRoot ? new Node(*other.m_pRoot) : nullptr;
    };

    ~BinaryTree() {
        delete m_pRoot;
    };

    void insert(const value_type &value, Ref ref){
        internal_insert(m_pRoot, value, ref);
    }
private:
    void internal_insert(NodePtr &pNode, const value_type &value, Ref ref, NodePtr parent = nullptr){
        if( !pNode ){
            pNode = new Node(value, ref);
            pNode -> m_Parent = parent;
            return;
        }
        size_t pos = !m_comp(value, pNode->getDataRef());
        internal_insert(pNode->getChildrenRef(pos), value, ref, pNode);
    }
};




#endif // __BINARY_TREE_H__