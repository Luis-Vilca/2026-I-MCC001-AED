#ifndef __HEAP_H__
#define __HEAP_H__
/**
 * @file heap.h
 * @brief Implementación de un Heap binario genérico.
 *
 * Este archivo define una estructura de datos Heap basada en un vector
 * dinámico, soportando inserción, extracción y acceso al elemento de
 * mayor o menor prioridad según el comparador especificado por los Traits.
 *
 * La implementación utiliza un arreglo implícito para representar el
 * árbol binario completo y mantiene la propiedad de Heap mediante las
 * operaciones de reorganización correspondientes.
 *
 * @author Luis Vilca
 */
#include <vector>
#include <mutex>
#include "vector.h"

/**
 * @class HeapNode
 * @brief Nodo utilizado por la estructura Heap.
 *
 * Almacena el dato y la referencia asociada a un elemento del Heap.
 *
 * @tparam Traits Traits que definen el tipo de dato almacenado.
 */
template <typename Traits>
class HeapNode{
    private:
        typename Traits::value_type m_data;
        Ref m_ref;
    public:
        HeapNode(typename Traits::value_type data, Ref ref) : m_data(data), m_ref(ref) {}
        
        typename Traits::value_type GetData() const { return m_data; }
        Ref GetRef()  const { return m_ref;  }
};

/**
 * @struct AscendingHeapTrait
 * @brief Traits para un Heap de prioridad ascendente.
 *
 * Define el tipo de nodo y utiliza el comparador std::less para
 * mantener la propiedad del Heap.
 *
 * @tparam T Tipo de dato almacenado.
 */
template <typename T>
struct AscendingHeapTrait : public BaseContainerTrait<T, HeapNode<T> >,
                            public AscendingTrait<T>
{
};

/**
 * @struct DescendingHeapTrait
 * @brief Traits para un Heap de prioridad descendente.
 *
 * Define el tipo de nodo y utiliza el comparador std::greater para
 * mantener la propiedad del Heap.
 *
 * @tparam T Tipo de dato almacenado.
 */
template <typename T>
struct DescendingHeapTrait : public BaseContainerTrait<T, HeapNode<T> >,
                            public DescendingTrait<T>
{
};

// Revisar: https://www.cs.usfca.edu/~galles/visualization/Heap.html
// Pero en este ejercicio empezamos el la posicion [0]

/**
 * @class Heap
 * @brief Implementa un Heap binario genérico basado en un arreglo dinámico.
 *
 * La clase proporciona operaciones de inserción, extracción y consulta
 * del elemento con mayor prioridad, manteniendo automáticamente la
 * propiedad de Heap mediante las operaciones de reorganización del árbol.
 *
 * El criterio de prioridad depende del comparador definido en los Traits.
 *
 * @tparam Traits Traits que definen el tipo de dato, el tipo de nodo y
 * el comparador utilizado por el Heap.
 */
template <typename Traits>
class Heap {
public:
    using value_type = typename Traits::value_type;
    using Node       = typename Traits::Node;
    using Comp       = typename Traits::Comp;
    using MySelf     = Heap<Traits>;
private:
    vector<Node> m_heap;
    Comp         m_comp;
public:

    void insert(const value_type &value, Ref ref) {
        m_heap.push_back( Node(value, ref) );
        heapify_up(m_heap.size() - 1);
    }

    // Revisar completamente
    void extract() {
        if (m_heap.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        if (m_heap.size() == 1) {
            m_heap.pop_back();
            return;
        }
        m_heap[0] = m_heap.back();
        m_heap.pop_back();
        heapify_down(0);
    }

    T peek_min() const {
        if (m_heap.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return m_heap[0];
    }

    bool empty() const {
        return m_heap.empty();
    }

    size_t size() const {
        return m_heap.size();
    }

private:
    void heapify_up(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if ( m_comp(m_heap[index], m_heap[parent]) ) {
                std::swap(m_heap[index], m_heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapify_down(size_t index) {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;
        size_t smallest = index;

        if (left < m_heap.size() && m_comp(m_heap[left], m_heap[smallest]) ) {
            smallest = left;
        }
        if (right < m_heap.size() && m_comp(m_heap[right], m_heap[smallest]) ) {
            smallest = right;
        }

        if (smallest != index) {
            std::swap(m_heap[index], m_heap[smallest]);
            heapify_down(smallest);
        }
    }
};

#endif // __HEAP_H__