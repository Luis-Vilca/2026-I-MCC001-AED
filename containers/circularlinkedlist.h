#ifndef __CIRCULARLINKEDLIST_H__
#define __CIRCULARLINKEDLIST_H__

#include "linkedlist.h"

template <typename T>
struct BaseCircularLinkedListTrait : public BaseContainerTrait<T, LLNode<T>>{

};

template <typename T>
struct AscendingCircularLinkedListTrait : public BaseCircularLinkedListTrait<T>{
    using Comp = less<T>;
};

template <typename T>
struct DescendingCircularLinkedListTrait : public BaseCircularLinkedListTrait<T>{
    using Comp = greater<T>;
};


template <typename Traits>
class CircularLinkedList : public LinkedList<Traits>{
public:
    using value_type = typename Traits::value_type;
    using Node       = typename Traits::Node;
    using Comp       = typename Traits::Comp;
    using MySelf     = CircularLinkedList<Traits>;

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
void CircularLinkedList<Traits>::internal_insert(Node* &pPrev, const value_type &value, Ref ref){
    
    scoped_lock<mutex> lock(m_mtx);
    if (!pPrev || m_comp(value, pPrev->getDataRef())) {
        Node* pTemp = new Node(value, ref, pPrev);
        pPrev = pTemp;
        ++m_size;
        if (pPrev == m_pRoot) {
            if (m_size == 1)
                m_pTail = m_pRoot;
        }
        if (pTemp->getNext() == nullptr)
            m_pTail = pTemp;
        
        m_pTail->setNext(m_pRoot);
        return;
    }
    internal_insert(pPrev->getNextRef(), value, ref);
}

#endif //__CIRCULARLINKEDLIST_H__