/**
 * @file doublelinkedlist.h
 * @brief Implementación de una lista doblemente enlazada genérica.
 *
 * Este archivo define una lista doblemente enlazada basada en plantillas,
 * permitiendo el recorrido en ambos sentidos mediante iteradores
 * hacia adelante y hacia atrás. La implementación extiende la
 * funcionalidad de LinkedList incorporando enlaces al nodo anterior.
 *
 * @author Luis Vilca
 */
#include "linkedlist.h"

/**
 * @class DLLNode
 * @brief Nodo utilizado por la clase DoubleLinkedList.
 *
 * Extiende el nodo de una lista enlazada simple agregando un puntero
 * al nodo anterior, permitiendo el recorrido bidireccional de la lista.
 *
 * @tparam T Tipo de dato almacenado en el nodo.
 */
template <typename T>
class DLLNode :  public LLNode<T>{
public:
    using value_type = T;
    using Node       = DLLNode<T>;
private:
    Node *m_pPrev;
public:
    DLLNode(T data, Ref ref, Node *pNext = nullptr, Node *pPrev = nullptr)
        : LLNode(data, ref, pNext), m_pPrev(pPrev){}

    Node*  getPrev() const { return m_pPrev; }
    Node*& getPrevRef() { return m_pPrev; }
    void   setPrev(Node* pPrev) { m_pPrev = pPrev; }
};

/**
 * @struct BaseDoubleLinkedListTrait
 * @brief Traits base para la implementación de una lista doblemente enlazada.
 *
 * Define el tipo de nodo utilizado por la estructura y sirve como base
 * para las variantes con orden ascendente y descendente.
 *
 * @tparam T Tipo de dato almacenado.
 */
template <typename T>
struct BaseDoubleLinkedListTrait : public BaseContainerTrait<T, DLLNode<T>>{

};

/**
 * @struct AscendingDoubleLinkedListTrait
 * @brief Traits para listas doblemente enlazadas con orden ascendente.
 *
 * Utiliza el comparador std::less para mantener los elementos en orden creciente.
 *
 * @tparam T Tipo de dato almacenado.
 */
template <typename T>
struct AscendingDoubleLinkedListTrait : public BaseDoubleLinkedListTrait<T>{
    using Comp = less<T>;
};

/**
 * @struct DescendingDoubleLinkedListTrait
 * @brief Traits para listas doblemente enlazadas con orden descendente.
 *
 * Utiliza el comparador std::greater para mantener los elementos en orden decreciente.
 *
 * @tparam T Tipo de dato almacenado.
 */
template <typename T>
struct DescendingDoubleLinkedListTrait : public BaseDoubleLinkedListTrait<T>{
    using Comp = greater<T>;
};

// Reutiizar el LinkedListForwardIterator de la linked list

/**
 * @class DoubleLinkedListBackwardIterator
 * @brief Iterador para recorrer una lista doblemente enlazada en sentido inverso.
 *
 * Permite recorrer la lista desde el último nodo hasta el primero
 * utilizando el framework de iteradores del proyecto.
 *
 * @tparam Container Tipo del contenedor asociado.
 */
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

/**
 * @class DoubleLinkedList
 * @brief Implementa una lista doblemente enlazada genérica.
 *
 * Esta clase hereda de LinkedList e incorpora enlaces al nodo anterior,
 * permitiendo recorridos bidireccionales y operaciones eficientes sobre
 * ambos extremos de la lista.
 *
 * @tparam Traits Traits que definen el tipo de dato, el comparador y
 * las propiedades del contenedor.
 */
template <typename Traits>
class DoubleLinkedList public LinkedList<Traits>{
public:
    using value_type = typename Traits::value_type;
    using Node       = typename Traits::Node;
    using Comp       = typename Traits::Comp;
    using MySelf     = DoubleLinkedList<Traits>;

    using forward_iterator  = LinkedListForwardIterator<MySelf>;
    using backward_iterator = DoubleLinkedListBackwardIterator<MySelf>;

private:
    Node *m_pHead;
    Node *m_pTail;
    size_t m_size;
    mutex  m_mtx;

public:
    DoubleLinkedList() : m_pHead(nullptr), m_pTail(nullptr), m_size(0) {}
    DoubleLinkedList(DoubleLinkedList &other){}
    DoubleLinkedList(DoubleLinkedList &&other){
        scoped_lock<mutex> lock(m_mtx);
        m_pHead = exchange(other.m_pHead, nullptr);
        m_pTail = exchange(other.m_pTail, nullptr);
        m_size = exchange(other.m_size, 0);
    }
    ~DoubleLinkedList() {
        scoped_lock<mutex> lock(m_mtx);
        while (m_pHead != nullptr) {
            Node *pTemp = m_pHead;
            m_pHead = m_pHead->getNextRef();
            delete pTemp;
        }
        m_pHead = nullptr;
        m_pTail = nullptr;
        m_size = 0;
    }
    
    size_t size () const { return m_size; }
    bool isEmpty() const { return m_pHead == nullptr; }
    
    voidd insert(value_type value, Ref ref){
        // TODO: insertar la el nodo hacia adelante (como en la LinkedList)
        // adicionalmente conectar el nodo anterior con su nuevo siguiente
        // usar internal insert pero debe devolver el nuevo nodo creado y 
        // el puntero al lnodo anterior
    }
    void push_back(value_type value, Ref ref);
    value_type pop_back();

    forward_iterator begin()   { return forward_iterator(this, m_pHead); }
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