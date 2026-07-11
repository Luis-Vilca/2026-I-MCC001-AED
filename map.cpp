#include <iostream>
#include <map>
#include "map.h"
#include "types.h"

using namespace std;

/**
 * @brief Muestra el uso del contenedor asociativo std::map
 *        que almacena pares clave-valor de forma ordenada
 * 
 */
void print_map(string_view comment, const map<string, TI>& m, ostream &os)
{
    os << comment;
    for (const auto& [key, value] : m)
        os << '[' << key << "] = " << value << "; ";
    os << '\n';
}

ostream& buscarEdad(const map<string, TI>& m, string nombre, ostream &os){
    os << "Buscar la edad de " << nombre << ": ";

    try{
        os << m.at(nombre) << endl;
    } 
    catch (const out_of_range& e){
        os << "La clave no existe.";
        os << "Excepcion: " << e.what() << endl;
    }

    return os;
}

ostream& tieneElementos(const map<string, TI>& m, ostream &os){
    if (m.empty() == 1)
        os << "El contenedor esta vacio"<< endl;
    else
        os << "El contenedor tiene elementos" << endl;
    return os;
}

void DemoMap(){
    cout << "\nDemoMap:" << endl;

    //Se crea un contenedor de tamaño dinámico
    map<string, TI> edades;

    edades["Pedro"]    = 24;
    edades["Santiago"] = 20;
    edades["Ana"]      = 21;

    //No se accede mediante un índice al valor, sino por una clave
    cout << edades["Ana"] << endl;

    //También se puede añadir con insert, si no exsite se crea uno nuevo
    edades.insert(make_pair("Jorge", 25));
    print_map("1) Contenedor de edades: " , edades, cout);

    //Si es que ya existe la clave, no hace nada
    edades.insert({"Pedro", 30});
    print_map("2) Contenedor de edades: " , edades, cout);

    //Pero se puede modificar los elementos de las claves creadas
    edades["Pedro"] = 30;
    print_map("3) Contenedor de edades: " , edades, cout);

    //Si se usa un elemento que no ha sido creado anteriormente, 
    //se inicializa con 0 (en el caso de int)
    cout << "Edad de Carlos: " << edades["Carlos"]<< endl;
    print_map("4) Contenedor de edades: " , edades, cout);

    //Por eso para buscar no es bueno usar el operador [], sino at()
    cout << "Buscar edad de Jorge: " << edades.at("Jorge") << endl;
    buscarEdad(edades, "Pedro", cout);

    //Sino lanza una excepcion
    buscarEdad(edades, "Marco", cout);

    //Para eliminar una clave se usa erase y para medir el tamaño del contenedor se usa size
    
    cout << "Tamanio del contenedor antes: " << edades.size() << endl;
    edades.erase("Carlos");
    cout << "Tamanio del contenedor despues: " << edades.size() << endl;
    print_map("5) Contenedor de edades: " , edades, cout);

    //Se usa clear para eliminar todos los elementos del contenedor
    tieneElementos(edades, cout);
    edades.clear();
    tieneElementos(edades, cout) << "Elementos del contenedor: " << edades.size();

}