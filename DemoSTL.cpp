#include <generator>
#include <iostream>
#include <vector>
#include "types.h"

using namespace std;

generator<TI64> generateNumbers(TI64 begin, TI64 end)
{
    for (TI64 i = begin; i <= end; ++i)
    {
        cout << "[Generator] " << i << endl;
        co_yield i;
    }
}

generator<TI64> filterEven(generator<TI64>& source)
{
    for (auto value : source)
    
        if (value % 2 == 0)
        {
            cout << "    [Filter] " << value << endl;
            co_yield value;
        }
    
}

generator<TI64> square(generator<TI64>& source)
{
    for (auto value : source)
    {
        cout << "        [Square] " << value << "^2" << endl;
        co_yield value * value;
    }
}

class Persona
{
    string nombre;

public:

    Persona(string n) : nombre(n) {}
    decltype(auto) getNombre(this auto&& self)
    {
        return (self.nombre);
    }
};


void DemoSTL(){
    //Demo generator
    cout << endl << "Demo generator: " << endl;
    auto nums = generateNumbers(1, 1000000);
    auto evens = filterEven(nums);
    auto result = square(evens);
    TI count = 0;
    for (auto value : result)
    {
        cout << "Resultado = " << value << endl;
        if (++count == 5)
            break;
    }
    cout << sizeof(nums) << endl;
    cout << sizeof(evens) << endl;
    cout << sizeof(result) << endl;
    
    cout << "sizeof(generator): " << sizeof(nums) << endl;

    vector<TI64> datos;

    for (TI64 i = 1; i <= 1000000; ++i)
        datos.push_back(i);

    cout << "sizeof(vector): " << sizeof(datos) << endl;
    cout << "Elementos almacenados: " << datos.size() << endl;
    cout << "Memoria reservada: " << datos.capacity() * sizeof(TI64)
        << " bytes" << endl;

    //Demo deducing sobre this
    cout << endl << "Demo deducing this: " << endl;
    Persona p("Luis");
    const Persona cp("Carlos");

    cout << p.getNombre() << endl;
    cout << cp.getNombre() << endl;

    p.getNombre() = "Pedro";

    cout << p.getNombre() << endl;
}
