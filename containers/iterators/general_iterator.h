#ifndef __ITERATOR_H__
#define __ITERATOR_H__
/**
 * @file general_iterator.h
 * @brief Implementación de la clase base para los iteradores del proyecto.
 *
 * Este archivo define un iterador genérico basado en el patrón
 * CRTP (Curiously Recurring Template Pattern), proporcionando la
 * funcionalidad común utilizada por los iteradores de las distintas
 * estructuras de datos implementadas en la biblioteca.
 *
 * @author Luis Vilca
 */
#include <algorithm>
#include <utility>
using namespace std;

/**
 * @class general_iterator
 * @brief Clase base para la implementación de iteradores genéricos.
 *
 * Esta clase proporciona la funcionalidad común para todos los
 * iteradores del proyecto, incluyendo el acceso al nodo actual,
 * desreferenciación, comparación y acceso mediante operador flecha.
 *
 * Se implementa utilizando el patrón CRTP, permitiendo que cada
 * iterador derivado defina su propio comportamiento de recorrido
 * mientras reutiliza la interfaz común.
 *
 * @tparam Container Tipo del contenedor asociado al iterador.
 * @tparam IteratorBase Clase derivada que implementa el comportamiento
 * específico del iterador.
 */
template <typename Container, class IteratorBase> // 
class general_iterator
{public:
    using Node   = typename Container::Node;
    using Myself = general_iterator<Container, IteratorBase>;
    
protected:
    Container *m_pContainer;
    Node      *m_pNode;

protected:
    IteratorBase& self() { return *(IteratorBase *)this;}
     
public:
    general_iterator(Container *pContainer, Node *pNode)
        : m_pContainer(pContainer), m_pNode(pNode) {}
    general_iterator(const Myself &other) 
          : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
    general_iterator(Myself &&other) // Move constructor
          {   m_pContainer = move(other.m_pContainer);
              m_pNode      = move(other.m_pNode);
          }
    IteratorBase operator=(IteratorBase &iter)
          {   m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return this->self(); // Pending static_cast?
          }
    Node *getNode() const { return m_pNode; }
    friend bool operator==(const IteratorBase &a, const IteratorBase &b) { return a.getNode() == b.getNode(); } 
    Node &operator*(){ return *m_pNode; }
    Node* operator->() const { return m_pNode; }
};

#endif
 