#ifndef __VECTOR_H__
#define __VECTOR_H__
    /**
     * @file vector.h
     * @brief Implementación de un contenedor dinámico tipo Vector.
     *
     * Este archivo define la clase plantilla Vector y sus iteradores asociados.
     * El contenedor almacena elementos de forma contigua en memoria,
     * incrementando automáticamente su capacidad cuando es necesario.
     *
     * Además, proporciona algoritmos genéricos para recorrer y buscar
     * elementos mediante iteradores compatibles con el proyecto.
     *
     * @author Luis Vilca
     */
#include <cstddef>
#include <sstream> 
#include <mutex>
#include "iterators/general_iterator.h"
#include "../types.h"
#include "../foreach.h"
#include "basetrait.h"

/**
 * @class vector_forward_iterator
 * @brief Iterador de avance para la clase Vector.
 *
 * Permite recorrer secuencialmente los elementos del vector desde el
 * primero hasta el último.
 *
 * @tparam Container Tipo del contenedor asociado.
 */
template <typename Container>
class vector_forward_iterator : public general_iterator<Container, vector_forward_iterator<Container>> {
public:
    using MySelf = vector_forward_iterator<Container>;
    using Parent = general_iterator<Container, MySelf>;
    using Parent::Parent;
    /**
     * @brief Avanza el iterador a la siguiente posición.
     *
     * @return Referencia al iterador actualizado.
     */
    MySelf operator++() { this->m_pNode++; return *this; }
};

/**
 * @class vector_backward_iterator
 * @brief Iterador inverso para la clase Vector.
 *
 * Permite recorrer el vector desde el último elemento hacia el primero.
 *
 * @tparam Container Tipo del contenedor asociado.
 */
template <typename Container>
class vector_backward_iterator : public general_iterator<Container, vector_backward_iterator<Container>> {
public:
    using MySelf = vector_backward_iterator<Container>;
    using Parent = general_iterator<Container, MySelf>;
    using Parent::Parent;
    /**
     * @brief Retrocede el iterador hacia el elemento anterior.
     *
     * @return Referencia al iterador actualizado.
     */
    MySelf operator++() { this->m_pNode--; return *this; }
};

/**
 * @struct VectorNode
 * @brief Nodo utilizado para almacenar los elementos del Vector.
 *
 * Cada nodo almacena un dato del tipo especificado por el usuario y
 * una referencia asociada, utilizada por el resto de estructuras del
 * proyecto.
 *
 * @tparam T Tipo del dato almacenado.
 */
template <typename T>
struct VectorNode{
    
    using value_type = T;
    value_type m_data;
    Ref          m_ref;
    /**
     * @brief Construye un nodo vacío.
     */
    VectorNode() : m_data(value_type()), m_ref(Ref()) {}
    /**
     * @brief Construye un nodo con un dato y una referencia.
     *
     * @param data Valor que será almacenado.
     * @param ref Referencia asociada al dato.
     */
    VectorNode(value_type data, Ref ref) : m_data(data), m_ref(ref) {}
    /**
     * @brief Convierte el contenido del nodo en una cadena.
     *
     * @return Representación textual del nodo.
     */
    string ToString(){
        ostringstream oss;
        oss << "(" << m_data << "," << m_ref << ")";
        return oss.str();
    }
    value_type   GetData() const { return m_data; }
    value_type&  GetDataRef()    { return m_data; }
    Ref GetRef()  const { return m_ref;  }
    /**
     * @brief Incrementa el dato almacenado.
     *
     * Sobrecarga el operador ++ definido para el tipo
     * almacenado.
     */
    void operator++() { ++m_data; }
    /**
     * @brief Incrementa el dato almacenado utilizando la
     * sobrecarga del operador +=.
     *
     * @param value Valor que será sumado al dato actual.
     */
    void operator+=(const value_type& value) { m_data += value; }
};

//Introducir el Node dentro de la clase vector
// template <typename T>
// struct VectorTraits : public BaseContainerTrait<T, VectorNode<T>>{
// };

/**
 * @brief Envía un nodo del vector a un flujo de salida.
 *
 * @tparam Traits Tipo del nodo.
 * @param os Flujo de salida.
 * @param vn Nodo que será escrito.
 *
 * @return Flujo de salida.
 */
template <typename Traits>
ostream& operator<<(ostream& os, VectorNode<Traits>& vn){
    return os << vn.ToString();
}

/**
 * @class Vector
 * @brief Implementa un contenedor dinámico basado en un arreglo.
 *
 * Esta clase almacena elementos de manera contigua en memoria y aumenta
 * automáticamente su capacidad cuando el espacio disponible resulta
 * insuficiente.
 *
 * Además, proporciona iteradores compatibles con los algoritmos
 * genéricos definidos en el proyecto.
 *
 * @tparam Traits Define el tipo de dato almacenado y las características
 *                del nodo utilizado por el contenedor.
 */
template <typename Traits>
class Vector{
    public:
        using value_type = typename Traits::value_type;
        using Node       = typename Traits::Node;
        using MySelf     = Vector<Traits>;
        using forward_iterator  = vector_forward_iterator<MySelf>;
        using backward_iterator = vector_backward_iterator<MySelf>;
    private:
        /// Arreglo dinámico donde se almacenan los elementos.
        Node * m_data;
        /// Número actual de elementos almacenados.
        size_t m_size;
        /// Capacidad total reservada.
        size_t m_capacity;
        /// Mutex utilizado para sincronizar accesos concurrentes.
        mutex  m_mtx;
    private:
        /**
         * @brief Incrementa la capacidad del almacenamiento interno.
         *
         * Reserva un nuevo bloque de memoria con mayor capacidad y copia todos
         * los elementos existentes.
         *
         * @note Complejidad O(n).
         */
        void resize();
    public:
        /**
         * @brief Construye un vector vacío.
         *
         * @param capacity Capacidad inicial del almacenamiento.
         */
        Vector(size_t capacity = 10);
        /**
         * @brief Libera toda la memoria utilizada por el vector.
         */
        ~Vector();
        /**
         * @brief Inserta un nuevo elemento al final del vector.
         *
         * Si la capacidad disponible no es suficiente, el vector incrementa
         * automáticamente su tamaño antes de insertar el nuevo elemento.
         *
         * @param value Valor a insertar.
         * @param ref Referencia asociada al valor.
         *
         * @note Complejidad amortizada O(1).
         */
        void push_back(value_type value, Ref ref);

        /**
         * @brief Obtiene el número de elementos almacenados.
         *
         * @return Cantidad de elementos del vector.
         */
        size_t size();
        /**
         * @brief Convierte el contenido completo del vector en una cadena.
         *
         * @return Representación textual del vector.
         */
        string ToString();

        /**
         * @brief Obtiene un iterador al primer elemento.
         *
         * @return Iterador de inicio.
         */
        forward_iterator  begin()   { return forward_iterator(this, m_data); }
        /**
         * @brief Obtiene un iterador al final del vector.
         *
         * @return Iterador de fin.
         */
        forward_iterator  end()     { return forward_iterator(this, m_data + m_size); }
        /**
         * @brief Obtiene un iterador inverso al último elemento.
         *
         * @return Iterador inverso de inicio.
         */
        backward_iterator rbegin()  { return backward_iterator(this, m_data + m_size - 1); }
        /**
         * @brief Obtiene un iterador inverso al elemento anterior al primero.
         *
         * @return Iterador inverso de fin.
         */
        backward_iterator rend()    { return backward_iterator(this, m_data - 1); }
        
        /**
         * @brief Ejecuta una función sobre cada elemento del vector.
         *
         * La función recibe cada nodo del vector en orden de recorrido.
         *
         * @tparam Func Tipo del objeto invocable.
         * @tparam Args Tipos de los argumentos adicionales.
         *
         * @param func Función que será ejecutada.
         * @param args Argumentos adicionales enviados a la función.
         */
        template <typename Func, typename... Args>
        void ForEach(Func func, Args &&... args){
            ::ForEach(begin(), end(), func, forward<Args>(args)...);
        }

        /**
         * @brief Ejecuta una función sobre cada elemento del vector en orden inverso.
         *
         * @tparam Func Tipo del objeto invocable.
         * @tparam Args Tipos de los argumentos adicionales.
         *
         * @param func Función que será ejecutada.
         * @param args Argumentos adicionales enviados a la función.
         */
        template <typename Func, typename... Args>
        void ReverseForEach(Func func, Args &&... args){
            ::ForEach(rbegin(), rend(), func, forward<Args>(args)...);
        }

        /**
         * @brief Busca el primer elemento que satisface un predicado.
         *
         * @tparam Func Tipo del predicado.
         * @tparam Args Tipos de argumentos adicionales.
         *
         * @param func Predicado utilizado durante la búsqueda.
         * @param args Argumentos adicionales del predicado.
         *
         * @return Iterador apuntando al primer elemento encontrado o `end()`
         *         si ningún elemento cumple la condición.
         */
        template <typename Func, typename... Args>
        forward_iterator FirstThat(Func func, Args &&... args){
            return ::FirstThat(begin(), end(), func, forward<Args>(args)...);
        }
        
        /**
         * @brief Busca el primer elemento que satisface un predicado recorriendo
         * el vector en sentido inverso.
         *
         * @tparam Func Tipo del predicado.
         * @tparam Args Tipos de argumentos adicionales.
         *
         * @param func Predicado utilizado durante la búsqueda.
         * @param args Argumentos adicionales.
         *
         * @return Iterador inverso al elemento encontrado o `rend()` si no existe.
         */
        template <typename Func, typename... Args>
        backward_iterator ReverseFirstThat(Func func, Args &&... args){
            return ::FirstThat(rbegin(), rend(), func, forward<Args>(args)...);
        }
};

template <typename Traits>
Vector<Traits>::Vector(size_t capacity) {
    m_data = nullptr;
    m_size = 0;
    m_capacity = capacity;
    m_data = new Node[m_capacity];
}

template <typename Traits>
Vector<Traits>::~Vector() {
    delete[] m_data;
}

template <typename Traits>
void Vector<Traits>::push_back(value_type value, Ref ref) {
    if (m_size == m_capacity)
        resize();
    scoped_lock<mutex> lock(m_mtx);
    m_data[m_size] = Node(value, ref);
    m_size++;
}

template <typename Traits>
void Vector<Traits>::resize() {
    scoped_lock<mutex> lock(m_mtx);
    m_capacity = m_capacity < 10 ? 10: m_capacity * 2;
    Node *newData = new Node[m_capacity];
    for (size_t i = 0; i < m_size; i++)
        newData[i] = m_data[i];
    delete[] m_data;
    m_data = newData;
}

template <typename Traits>
size_t Vector<Traits>::size() {
    return m_size;
}

// [1, 2, 3, 4, 5]
template <typename Traits>
string Vector<Traits>::ToString() {
    scoped_lock lock(m_mtx);
    ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < m_size-1; i++)
        oss << m_data[i] << ",";
    if (m_size > 0)
        oss << m_data[m_size-1];
    oss << "]";
    return oss.str();
}

template <typename Traits>
ostream& operator<<(ostream& os, Vector<Traits>& v){
    return os << v.ToString();
}

void DemoVector();
void DemoConcurrentVector();

#endif // __VECTOR_H__