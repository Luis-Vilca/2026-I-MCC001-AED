#ifndef __INORDER_ITERATOR_H__
#define __INORDER_ITERATOR_H__

/**
 * @file inorder_iterator.h
 * @brief Implementación de un iterador para el recorrido In-order de árboles binarios.
 *
 * Este archivo define un iterador genérico para recorrer árboles binarios
 * siguiendo el recorrido en orden (In-order). La dirección del recorrido
 * puede personalizarse mediante los parámetros de plantilla, permitiendo
 * reutilizar la misma implementación para recorridos de izquierda a derecha
 * o de derecha a izquierda.
 *
 * @author Luis Vilca
 */

#include "general_iterator.h"
using namespace std;

/**
 * @class InorderIterator
 * @brief Iterador genérico para realizar recorridos In-order sobre árboles binarios.
 *
 * Implementa el algoritmo de recorrido en orden utilizando la infraestructura
 * proporcionada por general_iterator. El comportamiento del recorrido se
 * parametriza mediante los índices de los hijos izquierdo y derecho,
 * permitiendo reutilizar la misma clase para recorridos en ambos sentidos.
 *
 * @tparam Container Tipo del contenedor asociado.
 * @tparam DerivedIterator Tipo del iterador derivado (CRTP).
 * @tparam First Índice del primer hijo a visitar.
 * @tparam Second Índice del segundo hijo a visitar.
 */
template <typename Container, typename DerivedIterator, size_t First, size_t Second>
class InorderIterator : public general_iterator<Container, DerivedIterator> {

    using Parent = general_iterator<Container, DerivedIterator>;
    using Parent::Parent;
    using NodePtr = typename Container::NodePtr;
    
public:

    static NodePtr first(NodePtr root) {
        while(root && root->getChild(First))
            root = root->getChild(First);
        return root;
    }

    // TODO: Completar el operator++
    DerivedIterator& operator++(){
        
        NodePtr node = this->m_pNode;

        if (!node)
            return this->self();
        if (node->getChild(Second)) {
            node = node->getChild(Second);
            while (node->getChild(First))
                node = node->getChild(First);
        }
        else {
            NodePtr parent = node->getParent();
            while (parent && node == parent->getChild(Second))
            {
                node = parent;
                parent = parent->getParent();
            }
            node = parent;
        }
        this->m_pNode = node;
        return this->self();
    }
};

#endif //__INORDER_ITERATOR_H__