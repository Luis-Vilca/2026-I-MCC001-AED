#ifndef __BTREE_ITERATOR_H__
#define __BTREE_ITERATOR_H__

/**
 * @file BTreeIterator.h
 * @brief Implementación de iteradores para el recorrido de árboles B.
 *
 * Este archivo define un iterador genérico para recorrer árboles B en
 * orden ascendente o descendente. El recorrido se realiza mediante una
 * pila de estados que mantiene el camino desde la raíz hasta el nodo
 * actual, permitiendo avanzar eficientemente entre los elementos del árbol.
 *
 * @author Luis Vilca
 */

#include <vector>
#include "general_iterator.h"

/**
 * @enum BTreeIteratorDirection
 * @brief Define el sentido de recorrido de un iterador de árbol B.
 *
 * Permite especificar si el recorrido se realiza en orden ascendente
 * (Forward) o descendente (Backward).
 */
enum class BTreeIteratorDirection { Forward, Backward };

/**
 * @class BTreeIterator
 * @brief Iterador genérico para recorrer un árbol B.
 *
 * Esta clase implementa un iterador basado en el patrón CRTP para recorrer
 * los elementos almacenados en un árbol B. El recorrido puede realizarse
 * en sentido ascendente o descendente, según la dirección especificada
 * mediante el parámetro de plantilla.
 *
 * Internamente mantiene una pila con el camino desde la raíz hasta el nodo
 * actual, lo que permite avanzar entre las claves del árbol sin necesidad
 * de realizar nuevamente la búsqueda desde la raíz.
 *
 * @tparam Container Tipo del contenedor asociado.
 * @tparam Direction Dirección del recorrido (Forward o Backward).
 */
template <typename Container,BTreeIteratorDirection Direction>
class BTreeIterator
    : public general_iterator<Container, BTreeIterator<Container, Direction>>
{
    using MySelf = BTreeIterator<Container, Direction>;
    using Parent = general_iterator<Container, MySelf>;

protected:
    using Node   = typename Container::Node;
    using BTPage = typename Container::BTNode;

    /**
     * @struct State
     * @brief Representa un nivel del recorrido dentro del árbol B.
     *
     * Almacena la página actual y el índice de la clave que está siendo
     * visitada, permitiendo reconstruir el camino desde la raíz durante
     * el recorrido del árbol.
     */
    struct State
    {
        BTPage* page;
        size_t index;
    };

    vector<State> m_path;

protected:
    void descend(BTPage* page);
    void setCurrent();

public:
    BTreeIterator(Container* container, BTPage* root)
        : Parent(container, nullptr)
    {
        descend(root);
        setCurrent();
    }

    MySelf& operator++()
    {
        next();
        return *this;
    }

private:
    void next();
};

template <typename Container,BTreeIteratorDirection Direction>
void BTreeIterator<Container, Direction>::descend(BTPage* page)
{
    while (page)
    {
        if constexpr (Direction == BTreeIteratorDirection::Forward)
        {
            m_path.push_back({page, 0});
            page = page->getSubPage(0);
        }
        else
        {
            size_t index = page->getKeyCount();
            m_path.push_back({page, index});
            page = page->getSubPage(index);
        }
    }
}

template <typename Container,BTreeIteratorDirection Direction>
void BTreeIterator<Container, Direction>::setCurrent()
{
    if (m_path.empty())
    {
        this->m_pNode = nullptr;
        return;
    }

    auto& state = m_path.back();

    if constexpr (Direction == BTreeIteratorDirection::Forward)
        this->m_pNode = state.page->getNode(state.index);
    else
        this->m_pNode = state.page->getNode(state.index - 1);
}

template <typename Container,BTreeIteratorDirection Direction>
void BTreeIterator<Container, Direction>::next()
{
    if (m_path.empty())
        return;

    auto& state = m_path.back();
    size_t childIndex;

    if constexpr (Direction == BTreeIteratorDirection::Forward)
        childIndex = state.index + 1;
    else
        childIndex = state.index - 1;

    BTPage* nextPage = state.page->getSubPage(childIndex);

    if constexpr (Direction == BTreeIteratorDirection::Forward)
        ++state.index;
    else
        --state.index;

    if (nextPage)
        descend(nextPage);
    else
    {
        while (!m_path.empty())
        {
            auto& current = m_path.back();
            bool finished;

            if constexpr (Direction == BTreeIteratorDirection::Forward)
                finished = current.index >=current.page->getKeyCount();
            else
                finished = current.index == 0;
            if (!finished)
                break;
            m_path.pop_back();
        }
    }

    setCurrent();
}
#endif //__BTREE_ITERATOR_H__
