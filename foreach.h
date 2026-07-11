#ifndef __FOREACH_H__
#define __FOREACH_H__
/**
 * @file foreach.h
 * @brief Algoritmos genéricos para recorrer contenedores mediante iteradores.
 *
 * Este archivo implementa un conjunto de algoritmos genéricos inspirados
 * en la STL, permitiendo recorrer contenedores, aplicar funciones sobre
 * sus elementos y realizar búsquedas mediante predicados utilizando
 * iteradores compatibles con la biblioteca.
 *
 * @author Luis Vilca
 */
#include <iostream>
#include <mutex>
#include <utility> // forward

using namespace std;

/**
 * @brief Aplica una función a todos los elementos de un rango de iteradores.
 */
template <typename Iterator, typename Func, typename... Args>
void ForEach(Iterator begin, Iterator end, Func func, Args&&... args)
{
    Iterate(begin, end, forward<Func>(func), forward<Args>(args)...);
}

/**
 * @brief Aplica una función a todos los elementos de un contenedor.
 */
template <typename Container, typename Func, typename... Args>
void ForEach(const Container& container, Func func, Args&&... args)
{
    scoped_lock<mutex> lock(container.getMutex());
    ForEach(container.begin(), container.end(), forward<Func>(func), forward<Args>(args)...);
}

/**
 * @brief Devuelve el primer elemento que satisface una condicion.
 */
template <typename Iterator, typename Func, typename... Args>
Iterator FirstThat(Iterator begin, Iterator end, Func func, Args&&... args)
{
    return Iterate(begin, end, forward<Func>(func), forward<Args>(args)...);
}

/**
 * @brief Devuelve el primer elemento que satisface una condicion en un contenedor.
 */
template <typename Container, typename Func, typename... Args>
auto FirstThat(const Container& container, Func func, Args&&... args)
{
    scoped_lock<mutex> lock(container.getMutex());
    return FirstThat(container.begin(), container.end(), forward<Func>(func), forward<Args>(args)...);
}

/**
 * @brief Implementa el algoritmo base utilizado por ForEach y FirstThat.
 */
template <typename Iterator, typename Func, typename... Args>
decltype(auto) Iterate(Iterator begin, Iterator end, Func func, Args&&... args)
{
    using result_type = invoke_result_t<Func, decltype(*begin), Args&...>;
    for (auto it = begin; it != end; ++it)
        if constexpr (is_void_v<result_type>)
            invoke(func, *it, args...);
        else
            if (invoke(func, *it, args...))
                return it;
    if constexpr (!is_void_v<result_type>)
        return end;
}
#endif // __FOREACH_H__