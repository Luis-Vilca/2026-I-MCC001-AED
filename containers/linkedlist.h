#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

/**
 * @file linkedlist.h
 * @brief Implementación de una lista enlazada genérica.
 *
 * Este archivo define una lista enlazada simple basada en plantillas,
 * junto con su nodo interno, su iterador de recorrido y métodos para
 * inserción, eliminación, búsqueda y recorrido mediante algoritmos
 * genéricos.
 *
 * @author Luis Vilca
 */

#include <iostream>
#include <cstddef>   // size_t
#include <string>
#include <sstream>
#include <mutex>     // mutex
#include "iterators/general_iterator.h"
#include "../util.h"
#include "../types.h"
#include "../foreach.h"
#include "basetrait.h"

using namespace std;

// Forward iterator
/**
 * @class LinkedListForwardIterator
 * @brief Iterador de recorrido hacia adelante para una lista enlazada.
 *
 * Permite recorrer secuencialmente los nodos de una lista enlazada
 * desde el primer elemento hasta el último utilizando el framework
 * de iteradores del proyecto.
 *
 * @tparam Container Tipo del contenedor asociado.
 */
template <typename Container>
class LinkedListForwardIterator : public general_iterator<Container, 
                                            LinkedListForwardIterator<Container>>{
    using MySelf = LinkedListForwardIterator<Container>;
    using Parent = general_iterator<Container, MySelf>;
    using Parent::Parent;
public:
    // TODO: Completar el operator++
    MySelf& operator++(){
        this->m_pNode = this->m_pNode->getNext();
        return *this;
    }
};

/**
 * @class LinkedList
 * @brief Implementa una lista enlazada simple ordenada.
 *
 * La clase proporciona almacenamiento dinámico mediante nodos enlazados,
 * soportando inserción, eliminación, acceso mediante iteradores y
 * algoritmos genéricos de recorrido y búsqueda.
 *
 * @tparam Traits Traits que definen el tipo de dato almacenado,
 * el comparador y demás propiedades del contenedor.
 */
template <typename Traits>
class LinkedList{
public:
    
    class LinkedListNode;
    using value_type = typename Traits::value_type;
    using Node       = LinkedListNode;
    using NodePtr    = Node*;
    using Comp       = typename Traits::Comp;
    using MySelf     = LinkedList<Traits>;

    using forward_iterator = LinkedListForwardIterator<MySelf>;

private:
    NodePtr m_pRoot = nullptr;
    NodePtr m_pTail = nullptr;
    size_t m_size = 0;
    Comp   m_comp;
    mutex m_mtx;

public:

    /**
     * @class LinkedListNode
     * @brief Nodo utilizado por la clase LinkedList.
     *
     * Almacena el dato, una referencia asociada y un puntero al siguiente
     * nodo de la lista enlazada.
     */
    class LinkedListNode{
    public:
        using value_type = typename Traits::value_type;
        using Node       = LinkedListNode;
        using NodePtr    = Node*;

    protected:
        value_type m_data;
        Ref        m_ref;
        NodePtr    m_pNext;

    public:

        LinkedListNode(): m_pNext(nullptr){}
        LinkedListNode(value_type data, Ref ref, NodePtr pNext = nullptr)
                : m_data(data), m_ref(ref), m_pNext(pNext){ }

        value_type  getData() const             { return m_data;}
        value_type& getDataRef()                {return m_data;}
        void        setData(value_type data)    { m_data = data;}
        Ref         getRef() const              { return m_ref;}
        Ref&        getRefRef()                 { return m_ref;}
        void        setRef(Ref ref)             { m_ref = ref;}
        NodePtr     getNext() const             { return m_pNext;}
        NodePtr&    getNextRef()                { return m_pNext;}
        void        setNext(NodePtr pNext)      { m_pNext = pNext;}
        
        friend ostream& operator<<(ostream& os, const LinkedListNode& node) {
            return os << "(" << node.m_data << ", " << node.m_ref << ")";
        }
    };

public:
    LinkedList() {}
    LinkedList(const LinkedList &other){ // Copy constructor

        NodePtr pTemp = other.m_pRoot;

        while(pTemp != nullptr){
            push_back(pTemp -> getData(), pTemp -> getRef());
            pTemp = pTemp->getNext();
        }
            
    }
    LinkedList(LinkedList &&other){ // Move constructor
        
        scoped_lock<mutex> lock(m_mtx);
        m_pRoot = exchange(other.m_pRoot, nullptr);
        m_pTail = exchange(other.m_pTail, nullptr);
        m_size = exchange(other.m_size, 0);

    }
    LinkedList& operator=(const LinkedList &other){ // Copy assignment operator
    }
    LinkedList& operator=(LinkedList &&other){ // Move assignment operator
    }
    
    virtual        ~LinkedList() {
        
        scoped_lock<mutex> lock(m_mtx);
        NodePtr pTemp = m_pRoot;

        while (pTemp){
            Node* pNext = pTemp->getNext();
            delete pTemp;
            pTemp = pNext;
        }

        m_pRoot = nullptr;
        m_pTail = nullptr;
        m_size  = 0;
    }

    virtual void   push_front(value_type value, Ref ref) {
        NodePtr pTemp = new Node(value, ref, m_pRoot);  //Se crea el Nodo temporal con los datos ingresados que apunta a m_pRoot
        
        scoped_lock<mutex> lock(m_mtx);
        m_pRoot = pTemp;                              //Se actualiza el nodo raiz
        if (m_size == 0)
            m_pTail = pTemp;                          //Si la lista esta vacia la cola tambien se debe actualizar
        ++m_size;
    }
    virtual auto    pop_front() -> pair<value_type, Ref>{ 
        
        scoped_lock<mutex> lock(m_mtx);
        if( m_pRoot ){
            NodePtr pTemp = m_pRoot;
            m_pRoot = m_pRoot->getNext();
            --m_size;
            return make_pair(pTemp->getData(), pTemp->getRef());
        }else
            throw out_of_range("pop_front(): empty list");
    }
    virtual void    push_back(value_type value, Ref ref){
        NodePtr pTemp = new Node(value, ref, nullptr);  //Como es el ultimo nodo no apunta a nada
        
        scoped_lock<mutex> lock(m_mtx);
        if (m_size == 0){
            m_pRoot = pTemp;
            m_pTail = pTemp; 
        } else {
            m_pTail->setNext(pTemp);
            m_pTail = pTemp;
        }                  
        ++m_size;
    }
    virtual auto    pop_back() -> pair<value_type, Ref>{
        
        scoped_lock<mutex> lock(m_mtx);
        if( !m_pRoot )
            throw out_of_range("pop_back(): empty list");
        
        //Con un solo elemento
        if( m_pRoot == m_pTail){
            auto pDelete = make_pair(m_pTail->getData(), m_pTail->getRef());

            delete m_pTail;
            m_pRoot = nullptr;
            m_pTail = nullptr;
            
            --m_size;
            return pDelete;
        }

        //Lista con varios elementos
        NodePtr pTemp = m_pRoot;

        while (pTemp->getNext() != m_pTail)             //Recorrer toda la lista hasta el penultimo elemento
            pTemp = pTemp->getNext();

        auto pDelete = make_pair(m_pTail->getData(), m_pTail->getRef());
        
        delete pTemp->getNext();
        pTemp->setNext(nullptr);

        m_pTail = pTemp;
        --m_size;
        return pDelete;
    }
private:
            void    internal_insert(NodePtr &pParent, const value_type &value, Ref ref);
public:
    virtual void    insert(const value_type &value, Ref ref);
    
    virtual Node& operator[](const size_t index) const{
        
        if (index >= m_size)
        throw out_of_range("Index out of range");

        NodePtr pTemp = m_pRoot;
        for (size_t i = 0; i < index; ++i){
            pTemp = pTemp -> getNext();
        }
        return *pTemp;
    };

    virtual size_t  size() const { return m_size; }
    virtual string  toString();

    forward_iterator begin() { return forward_iterator(this, m_pRoot); }
    forward_iterator end()   { return forward_iterator(this, nullptr); }

    // Agregar Foreach
    template <typename Func, typename... Args>
    void ForEach(Func func, Args &&...  args){
        unique_lock<mutex> lock(m_mtx);
        ::ForEach(begin(), end(), func, std::forward<Args>(args)... );
    }

    //Agregar FirstThat
    template <typename Func, typename... Args>
    forward_iterator FirstThat(Func func, Args &&...  args){
        return ::FirstThat(begin(), end(), func, std::forward<Args>(args)... );
    }
};

template <typename Traits>
void LinkedList<Traits>::internal_insert(NodePtr &pPrev, const value_type &value, Ref ref){
    if(!pPrev || m_comp(value, pPrev->getDataRef())){
        pPrev = new Node(value, ref, pPrev);
        m_size++;
        if(pPrev == m_pRoot)
            m_pTail = pPrev;
        return;
    }
    internal_insert(pPrev->getNextRef(), value, ref);
}

template <typename Traits>
void LinkedList<Traits>::insert(const value_type &value, Ref ref){
    internal_insert(m_pRoot, value, ref);
}

template <typename Traits>
string  LinkedList<Traits>::toString() {
    stringstream ss;
    NodePtr pNode = m_pRoot;
    ss << "[";
    if( m_size > 0 ){
        for( size_t i = 0 ; i < size()-1 ; ++i ){
            ss << *pNode << ",";
            pNode = pNode->getNext();
        }
        ss << *pNode;
    }
    ss << "]";
    return ss.str();
}

template <typename Traits>
ostream& operator<<(ostream& os, LinkedList<Traits>& list){
    return os << list.toString();
}

template <typename Traits>
istream& operator>>(istream& is, LinkedList<Traits>& list){
    using value_type = typename LinkedList<Traits>::value_type;
    string line;

    getline(is, line);

    for (char& c : line){
        if (c == '[' || c == ']' || c == '(' || c == ')' || c == ',')
            c = ' ';
    }

    value_type value;
    Ref ref;
    stringstream ss(line);

    while (ss >> value >> ref){
        list.push_back(value, ref);
    }

    return is;
}

#endif // __LINKEDLIST_H__