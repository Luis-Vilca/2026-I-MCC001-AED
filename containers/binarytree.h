#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__
#include <iostream>
#include <cstddef>   // size_t
#include <string>
#include <sstream>
#include <mutex>
#include "general_iterator.h"
#include "basetrait.h"
#include "../foreach.h"
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
        
        using NodePtr = typename Container::NodePtr;
        NodePtr node = this->m_pNode;

        if (!node)
            return *this;
        if (node->getChild(1)) {
            node = node->getChild(1);
            while (node->getChild(0))
                node = node->getChild(0);
        }
        else {
            NodePtr parent = node->getParent();
            while (parent && node == parent->getChild(1))
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

template <typename Container>
class BinaryTreeBackwardInorderIterator : public general_iterator<Container, 
                                         BinaryTreeBackwardInorderIterator<Container>>{
    using MySelf = BinaryTreeBackwardInorderIterator<Container>;
    using Parent = general_iterator<Container, MySelf>;
    using Parent::Parent;
public:
    // TODO: Completar el operator++
    MySelf& operator++(){
        using NodePtr = typename Container::NodePtr;
        NodePtr node = this->m_pNode;

        if (!node)
            return *this;
        if (node->getChild(0)) {
            node = node->getChild(0);
            while (node->getChild(1))
                node = node->getChild(1);
        }
        else {
            NodePtr parent = node->getParent();
            while (parent && node == parent->getChild(0))
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

template <typename Container>
class BinaryTreeForwardPreorderIterator : public general_iterator<Container, 
                                         BinaryTreeForwardPreorderIterator<Container>>{
    using MySelf = BinaryTreeForwardPreorderIterator<Container>;
    using Parent = general_iterator<Container, MySelf>;
    using Parent::Parent;
public:
    // TODO: Completar el operator++
    MySelf& operator++(){
        using NodePtr = typename Container::NodePtr;
        NodePtr node = this->m_pNode;

        if (!node)
            return *this;
        if (node->getChild(0)) 
            node = node->getChild(0);
        else if (node->getChild(1))
            node = node->getChild(1);
        else {
            NodePtr parent =
                node->getParent();

            while (parent) {
                if (node == parent->getChild(0) && parent->getChild(1))
                {
                    node = parent->getChild(1);
                    break;
                }
                node = parent;
                parent = parent->getParent();
            }
            if (!parent)
                node = nullptr;
        }
        this->m_pNode = node;
        return *this;
    }
};

template <typename Container>
class BinaryTreeBackwardPreorderIterator : public general_iterator<Container, 
                                         BinaryTreeBackwardPreorderIterator<Container>>{
    using MySelf = BinaryTreeBackwardPreorderIterator<Container>;
    using Parent = general_iterator<Container, MySelf>;
    using Parent::Parent;
public:
    // TODO: Completar el operator++
    MySelf& operator++(){
        using NodePtr = typename Container::NodePtr;
        NodePtr node = this->m_pNode;

        if (!node)
            return *this;
        NodePtr parent = node->getParent();
        if (!parent) {
            this->m_pNode = nullptr;
            return *this;
        }
        if (node == parent->getChild(1) &&
            parent->getChild(0))
        {
            node = parent->getChild(0);
            while (true) {
                if (node->getChild(1))
                    node = node->getChild(1);
                else if (node->getChild(0))
                    node = node->getChild(0);
                else
                    break;
            }
        }

        else {
            node = parent;
        }
        this->m_pNode = node;
        return *this;
    }
};

template <typename Container>
class BinaryTreeForwardPostorderIterator : public general_iterator<Container, 
                                         BinaryTreeForwardPostorderIterator<Container>>{
    using MySelf = BinaryTreeForwardPostorderIterator<Container>;
    using Parent = general_iterator<Container, MySelf>;
    using Parent::Parent;
public:
    // TODO: Completar el operator++
    MySelf& operator++(){
        using NodePtr =
        typename Container::NodePtr;

        NodePtr node = this->m_pNode;
        if (!node)
            return *this;
        NodePtr parent = node->getParent();

        if (!parent) {
            this->m_pNode = nullptr;
            return *this;
        }

        if (node == parent->getChild(0) &&
            parent->getChild(1))
        {
            node = parent->getChild(1);
            while (true) {
                if (node->getChild(0))
                    node = node->getChild(0);
                else if (node->getChild(1))
                    node = node->getChild(1);
                else
                    break;
            }
        }
        else 
            node = parent;

        this->m_pNode = node;
        return *this;
    }
};

template <typename Container>
class BinaryTreeBackwardPostorderIterator : public general_iterator<Container, 
                                         BinaryTreeBackwardPostorderIterator<Container>>{
    using MySelf = BinaryTreeBackwardPostorderIterator<Container>;
    using Parent = general_iterator<Container, MySelf>;
    using Parent::Parent;
public:
    // TODO: Completar el operator++
    MySelf& operator++(){
        using NodePtr = typename Container::NodePtr;
        NodePtr node = this->m_pNode;

        if (!node)
            return *this;
        if (node->getChild(1)) 
            node = node->getChild(1);
        else if (node->getChild(0))
            node = node->getChild(0);
        else {
            NodePtr parent = node->getParent();
            while (parent) {
                if (node == parent->getChild(1) &&
                    parent->getChild(0))
                {
                    node = parent->getChild(0);
                    break;
                }
                node = parent;
                parent = parent->getParent();
            }
            if (!parent)
                node = nullptr;
        }
        this->m_pNode = node;
        return *this;
    }
};

template <typename T>
struct BaseBinaryTreeListTrait {
    using value_type = T;
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
    
    forward_inorder_iterator begin() {
        NodePtr node = m_pRoot;
        while (node && node->getChild(0))
            node = node->getChild(0);
        return forward_inorder_iterator(this, node);
    }
    
    forward_inorder_iterator end() {
        return forward_inorder_iterator(this, nullptr);
    }
    
    backward_inorder_iterator rbegin() {
        NodePtr node = m_pRoot;
        while (node && node->getChild(1))
            node = node->getChild(1);
        return backward_inorder_iterator(this, node);
    }
    
    backward_inorder_iterator rend() {
        return backward_inorder_iterator(this, nullptr);
    }
    
    forward_preorder_iterator begin_preorder() {
        return forward_preorder_iterator(this, m_pRoot);
    }
    
    forward_preorder_iterator end_preorder() {
        return forward_preorder_iterator(this, nullptr);
    }
    
    backward_preorder_iterator rbegin_preorder() {
        NodePtr node = m_pRoot;
        while (node) {
            if (node->getChild(1))
                node = node->getChild(1);
            else if (node->getChild(0))
                node = node->getChild(0);
            else
                break;
        }
        return backward_preorder_iterator(this, node);
    }
    
    backward_preorder_iterator rend_preorder() {
        return backward_preorder_iterator(this, nullptr);
    }
    
    forward_postorder_iterator begin_postorder() {
        NodePtr node = m_pRoot;
        while (node) {
            if (node->getChild(0))
                node = node->getChild(0);
            else if (node->getChild(1))
                node = node->getChild(1);
            else
                break;
        }
        return forward_postorder_iterator(this, node);
    }

    forward_postorder_iterator end_postorder() {
        return forward_postorder_iterator(this, nullptr);
    }

    backward_postorder_iterator rbegin_postorder(){
        return backward_postorder_iterator(this, m_pRoot);
    }

    backward_postorder_iterator rend_postorder(){
        return backward_postorder_iterator(this, nullptr);
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

public:
    template <typename Iterator, typename Func, typename... Args>
    void ForEach(Iterator begin, Iterator end, Func func, Args &&... args){
        scoped_lock<mutex> lock(m_mtx);
        for (auto it = begin; it != end; ++it)
            func(*it, forward<Args>(args)...);
    }

    template <typename Iterator, typename Func, typename... Args>
    Iterator FirstThat(Iterator begin, Iterator end, Func func, Args &&... args){
        scoped_lock<mutex> lock(m_mtx);
        for (auto it = begin; it != end; ++it){
            if (func(*it, forward<Args>(args)...))
                return it;
        }
        return end;
    }
};

#endif // __BINARY_TREE_H__