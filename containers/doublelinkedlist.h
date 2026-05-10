#ifndef __DOUBLELINKEDLIST_H__
#define __DOUBLELINKEDLIST_H__
#include "linkedlist.h"

template <typename T>
class DLLNode :  public LLNode<T>{
public:
    using value_type = T;
    using Node       = DLLNode<T>;
private:
    Node *m_pPrev;
public:
    DLLNode(T data, Ref ref, Node *pNext = nullptr, Node *pPrev = nullptr)
        : LLNode<T>(data, ref, pNext), m_pPrev(pPrev){}

    Node*  getNext() const {return static_cast<Node*>(LLNode<T>::getNext());}
    Node*& getNextRef() {return reinterpret_cast<Node*&>(LLNode<T>::getNextRef());}
    Node*  getPrev() const { return m_pPrev; }
    Node*& getPrevRef() { return m_pPrev; }
    void   setPrev(Node* pPrev) { m_pPrev = pPrev; }
};

template <typename T>
struct BaseDoubleLinkedListTrait : public BaseContainerTrait<T, DLLNode<T>>{

};

template <typename T>
struct AscendingDoubleLinkedListTrait : public BaseDoubleLinkedListTrait<T>{
    using Comp = less<T>;
};

template <typename T>
struct DescendingDoubleLinkedListTrait : public BaseDoubleLinkedListTrait<T>{
    using Comp = greater<T>;
};

// Reutiizar el LinkedListForwardIterator de la linked list

// Backward iterator
template <typename Container>
class DoubleLinkedListBackwardIterator : public general_iterator<Container, 
                                        DoubleLinkedListBackwardIterator<Container>>{
    using MySelf = DoubleLinkedListBackwardIterator<Container>;
    using Parent = general_iterator<Container, MySelf>;
    using Parent::Parent;
public:
    MySelf& operator++(){
        this->m_pNode = this->m_pNode->getPrev();
        return *this;
    }
};

template <typename Traits>
class DoubleLinkedList : public LinkedList<Traits>{
public:
    using value_type = typename Traits::value_type;
    using Node       = typename Traits::Node;
    using Comp       = typename Traits::Comp;
    using MySelf     = DoubleLinkedList<Traits>;
    using LinkedList<Traits>::m_pRoot;
    using LinkedList<Traits>::m_pTail;
    using LinkedList<Traits>::m_size;

    using forward_iterator  = LinkedListForwardIterator<MySelf>;
    using backward_iterator = DoubleLinkedListBackwardIterator<MySelf>;

private:
    mutex  m_mtx;
    mutex  m_mtx2;

private:
    void    internal_insert(Node* &pParent, const value_type &value, Ref ref);
public:
    DoubleLinkedList() {m_pRoot = nullptr; m_pTail = nullptr; m_size=0;}
    DoubleLinkedList(const DoubleLinkedList &other){
        
        scoped_lock<mutex> lock(m_mtx2);
        Node* pTemp = other.m_pRoot;

        while(pTemp != nullptr){
            push_back(pTemp -> getData(), pTemp -> getRef());
            pTemp = pTemp->getNext();
        }
    }
    DoubleLinkedList(DoubleLinkedList &&other){
        scoped_lock<mutex> lock(m_mtx);
        m_pRoot = exchange(other.m_pRoot, nullptr);
        m_pTail = exchange(other.m_pTail, nullptr);
        m_size = exchange(other.m_size, 0);
    }
    
    size_t size () const { return m_size; }
    bool isEmpty() const { return m_pRoot == nullptr; }
     
    // TODO: insertar la el nodo hacia adelante (como en la LinkedList)
    // adicionalmente conectar el nodo anterior con su nuevo siguiente
    // usar internal insert pero debe devolver el nuevo nodo creado y 
    // el puntero al lnodo anterior
    
    virtual void    insert(const value_type &value, Ref ref){
        scoped_lock<mutex> lock(m_mtx);
        LinkedList<Traits>::insert(value, ref);

        Node* pPrev = nullptr;
        Node* pCurrent = m_pRoot;
        while (pCurrent != nullptr){
            pCurrent->setPrev(pPrev);
            pPrev = pCurrent;
            pCurrent = pCurrent->getNext();
        }
        m_pTail = pPrev;
    };
       

    void push_back(value_type value, Ref ref){
       
        scoped_lock<mutex> lock(m_mtx);
        Node* pTemp = new Node(value, ref, nullptr, m_pTail); //Ultimo nodo
        
        if (m_size == 0){
            m_pRoot = pTemp;
            m_pTail = pTemp; 
        } else {
            m_pTail->setNext(pTemp);
            pTemp -> setPrev(m_pTail);
            m_pTail = pTemp;
        }                  
        ++m_size;
    };
    //value_type pop_back(); El retorno debe coincidir

    forward_iterator begin()   { return forward_iterator(this, m_pRoot); }
    forward_iterator end()     { return forward_iterator(this, nullptr); }
    backward_iterator rbegin() { return backward_iterator(this, m_pTail); }
    backward_iterator rend()   { return backward_iterator(this, nullptr); }

    template <typename Func, typename... Args>
    void ForEach(Func func, Args &&... args){
        scoped_lock<mutex> lock(m_mtx);
        ::ForEach(begin(), end(), func, forward<Args>(args)...);
    }

    template <typename Func, typename... Args>
    void ReverseForEach(Func func, Args &&... args){
        scoped_lock<mutex> lock(m_mtx);
        ::ForEach(rbegin(), rend(), func, forward<Args>(args)...);
    }

    template <typename Func, typename... Args>
    forward_iterator FirstThat(Func func, Args &&... args){
        scoped_lock<mutex> lock(m_mtx);
        return ::FirstThat(begin(), end(), func, forward<Args>(args)...);
    }

    template <typename Func, typename... Args>
    backward_iterator ReverseFirstThat(Func func, Args &&... args){
        scoped_lock<mutex> lock(m_mtx);
        return ::FirstThat(rbegin(), rend(), func, forward<Args>(args)...);
    }
};

#endif //__DOUBLELINKEDLIST_H__ 