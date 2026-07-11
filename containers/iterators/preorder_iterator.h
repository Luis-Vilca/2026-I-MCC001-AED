#ifndef __PREORDER_ITERATOR_H__
#define __PREORDER_ITERATOR_H__

/**
 * @file preorder_iterator.h
 * @brief Implementación de un iterador para el recorrido Pre-order de árboles binarios.
 *
 * Este archivo define un iterador genérico para recorrer árboles binarios
 * siguiendo el recorrido en preorden (Pre-order). La dirección del recorrido
 * puede configurarse mediante parámetros de plantilla, permitiendo recorrer
 * el árbol tanto de izquierda a derecha como en sentido inverso utilizando
 * la misma implementación.
 *
 * @author Luis Vilca
 */

#include "general_iterator.h"

/**
 * @class PreorderIterator
 * @brief Iterador genérico para realizar recorridos Pre-order sobre árboles binarios.
 *
 * Implementa el recorrido en preorden utilizando la infraestructura de
 * general_iterator. El nodo actual es visitado antes que sus subárboles,
 * y el orden de visita de los hijos se determina mediante los parámetros
 * de plantilla.
 *
 * @tparam Container Tipo del contenedor asociado.
 * @tparam DerivedIterator Tipo del iterador derivado (CRTP).
 * @tparam First Índice del primer hijo que será recorrido.
 * @tparam Second Índice del segundo hijo que será recorrido.
 */
template <typename Container, typename DerivedIterator, size_t First, size_t Second>
class PreorderIterator : public general_iterator<Container, DerivedIterator> {
    
    using Parent = general_iterator<Container, DerivedIterator>;
    using Parent::Parent;
    using NodePtr = typename Container::NodePtr;
    
public:

    static NodePtr first(NodePtr root){
        return root;
    }

    DerivedIterator& operator++() {

        NodePtr node = this->m_pNode;

        if (!node) 
            return this->self();

        if (node->getChild(First))
            node = node->getChild(First);

        else if (node->getChild(Second))
            node = node->getChild(Second);

        else {
            NodePtr parent = node->getParent();
            while (parent) {
                if (node == parent->getChild(First) && parent->getChild(Second)) {
                    node = parent->getChild(Second);
                    break;
                }
                node   = parent;
                parent = parent->getParent();
            }
            if (!parent) 
                node = nullptr;
        }
        this->m_pNode = node;
        return this->self();
    }
};

#endif // __PREORDER_ITERATOR_H__