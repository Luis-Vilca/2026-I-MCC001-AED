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

public:
    ~CircularDoubleLinkedList() override{
        if(m_pRoot != nullptr && m_pTail != nullptr){
            m_pTail->setNext(nullptr);
            m_pRoot->setPrev(nullptr);
        }
    }

    virtual void    insert(const value_type &value, Ref ref){
        
        scoped_lock<mutex> lock(m_mtx);
        if (m_pTail)
            m_pTail->setNext(nullptr);
        if(m_pRoot)
            m_pRoot->setPrev(nullptr);
        DoubleLinkedList<Traits>::insert(value, ref);
        if (m_pRoot && m_pTail){
            m_pTail->setNext(m_pRoot);
            m_pRoot->setPrev(m_pTail);
        }
    };
       
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

#endif //__CIRCULARDOUBLELINKEDLIST_H__