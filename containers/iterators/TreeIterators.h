#ifndef __TREE_VIEW_H__
#define __TREE_VIEW_H__
/**
 * @file TreeIterators.h
 * @brief Define una vista iterable para recorrer estructuras de datos basadas en árboles.
 *
 * Este archivo implementa una clase contenedora que encapsula un par de
 * iteradores (inicio y fin) junto con un mecanismo de sincronización,
 * permitiendo recorrer árboles de forma compatible con los bucles
 * basados en rango de C++ (`range-based for`).
 *
 * @author Luis Vilca
 */


/**
 * @class TreeIterators
 * @brief Representa una vista iterable sobre un recorrido de un árbol.
 *
 * Esta clase encapsula el iterador inicial y final de un recorrido
 * específico de un árbol, proporcionando una interfaz compatible con
 * los algoritmos de la STL y los bucles `range-based for`.
 *
 * Además, mantiene una referencia al mutex del contenedor para permitir
 * el acceso sincronizado durante el recorrido cuando sea necesario.
 *
 * @tparam IteratorType Tipo de iterador utilizado para recorrer el árbol.
 */
template <typename IteratorType>
class TreeIterators{
    IteratorType m_begin;
    IteratorType m_end;
    mutex* m_pMtx;
public:
    TreeIterators(IteratorType b, IteratorType e,  mutex* pMutex): m_begin(b), m_end(e), m_pMtx(pMutex) {}

    mutex& getMutex()    const { return *m_pMtx;}
    IteratorType begin() const { return  m_begin; }
    IteratorType end()   const { return  m_end;   }
};

#endif // __TREE_VIEW_H__