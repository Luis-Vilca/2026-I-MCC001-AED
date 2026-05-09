#ifndef __CIRCULARDOUBLELINKEDLIST_H__
#define __CIRCULARDOUBLELINKEDLIST_H__

#include "doublelinkedlist.h"

template <typename T>
struct BaseCircularDoubleLinkedListTrait : public BaseContainerTrait<T, DLLNode<T>>{

};

template <typename T>
struct AscendingCircularDoubleLinkedListTrait : public BaseCircularDoubleLinkedListTrait<T>{
    using Comp = less<T>;
};

template <typename T>
struct DescendingCircularDoubleLinkedListTrait : public BaseCircularDoubleLinkedListTrait<T>{
    using Comp = greater<T>;
};


template <typename Traits>
class CircularDoubleLinkedList : public DoubleLinkedList<Traits>{
public:
    using value_type = typename Traits::value_type;
    using Node       = typename Traits::Node;
    using Comp       = typename Traits::Comp;
    using MySelf     = CircularDoubleLinkedList<Traits>;

    using LinkedList<Traits>::m_pRoot;
    using LinkedList<Traits>::m_pTail;
    using LinkedList<Traits>::m_size;
private:
    mutex  m_mtx;
    mutex  m_mtx2;
private:
    void internal_insert(Node* &pParent, const value_type &value, Ref ref);

public:
    
    // Agregar Foreach
    template <typename Func, typename... Args>
    void ForEach(Func func, Args &&...  args){
        scoped_lock<mutex> lock(m_mtx);

        Node* pTemp = m_pRoot;

        for(size_t i = 0; i < m_size; ++i){
            func(*pTemp, forward<Args>(args)...);
            pTemp = pTemp->getNext();
        }
       
    }
};

template <typename Traits>
void CircularDoubleLinkedList<Traits>::internal_insert(Node* &pPrev, const value_type &value, Ref ref){
    
    scoped_lock<mutex> lock(m_mtx);
    if (!pPrev || m_comp(value, pPrev->getDataRef())) {
        if (m_size == 0) {
            Node* pTemp = new Node(value, ref, nullptr, nullptr);
            m_pRoot = pTemp;            
            m_pTail = pTemp;
            pTemp->setNext(pTemp);      
            pTemp->setPrev(pTemp);
            pPrev = pTemp;
            ++m_size;
            return;
        }

        Node* pTemp = new Node(value, ref, pPrev);
        pPrev->setNext(pTemp);
        pPrev->setPrev(pTemp);

        if (pPrev == m_pRoot)
            m_pRoot = pTemp;
        pPrev = pTemp;
        ++m_size;
        return;
    }

    if (pPrev->getNext() == m_pRoot) {
        Node* pTemp = new Node(value, ref, m_pRoot, m_pTail);
        m_pTail->setNext(pTemp);
        m_pRoot->setPrev(pTemp);
        m_pTail = pTemp;
        ++m_size;
        return;
    }
    internal_insert(pPrev->getNextRef(), value, ref);
}

#endif //__CIRCULARDOUBLELINKEDLIST_H__