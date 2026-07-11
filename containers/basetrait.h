#ifndef __BASE_TRAIT_H__
#define __BASE_TRAIT_H__
/**
 * @file basetrait.h
 * @brief Define los Traits utilizados por los contenedores de la biblioteca.
 *
 * Este archivo proporciona las estructuras base para describir las
 * características de los contenedores genéricos, incluyendo el tipo de
 * dato almacenado, el tipo de nodo y el comparador utilizado para
 * mantener el orden de los elementos.
 *
 * Los Traits permiten desacoplar la implementación de los contenedores
 * de los tipos de datos específicos, favoreciendo la reutilización del
 * código mediante programación genérica.
 *
 * @author Luis Vilca
 */
#include <functional> // less, greater

using namespace std;

/**
 * @struct ContainerTrait
 * @brief Traits base para contenedores genéricos.
 *
 * Define el tipo de dato almacenado, el tipo de referencia asociado y
 * el comparador utilizado por un contenedor.
 *
 * @tparam T Tipo de dato almacenado.
 * @tparam Ref Tipo de referencia asociado al dato.
 * @tparam Comparator Comparador utilizado para ordenar los elementos.
 */
template <typename T, typename Ref = void, typename Comparator = less<T>>
struct ContainerTrait{
    using value_type = T;
    using ref_type   = Ref;
    using Comp       = Comparator;
};

template <typename T>
using AscendingContainerTrait = ContainerTrait<T>;

template <typename T>
using DescendingContainerTrait = ContainerTrait<T, void , greater<T>>;

template <typename T, typename Ref>
using AscendingMapContainerTrait = ContainerTrait<T, Ref>;

template <typename T, typename Ref>
using DescendingMapContainerTrait = ContainerTrait<T, Ref , greater<T>>;

/**
 * @struct NodeContainerTrait
 * @brief Traits para contenedores cuyo tipo de nodo se especifica explícitamente.
 *
 * Esta estructura define el tipo de dato almacenado, el tipo de nodo
 * utilizado por el contenedor y el comparador empleado para ordenar
 * los elementos.
 *
 * @tparam T Tipo de dato almacenado.
 * @tparam NodeType Tipo de nodo utilizado por el contenedor.
 * @tparam Comparator Comparador utilizado para ordenar los elementos.
 */
template <typename T, typename NodeType, typename Comparator = less<T>>
struct NodeContainerTrait{
    using value_type = T;
    using Node       = NodeType;
    using Comp       = Comparator;
};

template <typename T, typename NodeType>
using AscendingNodeContainerTrait = NodeContainerTrait<T, NodeType, less<T>>;

template <typename T, typename NodeType>
using DescendingNodeContainerTrait = NodeContainerTrait<T, NodeType, greater<T>>;

#endif // __BASE_TRAIT_H__