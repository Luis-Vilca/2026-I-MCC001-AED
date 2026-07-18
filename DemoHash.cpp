#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include "types.h"

using namespace std;

// Calcula P(X=k) para una Poisson(lambda)
TD poisson(TI k, TD lambda){
    return exp(-lambda) * pow(lambda, k) / tgamma(k + 1);
}

// Imprime el contenido de la tabla hash
void printTable(const vector<vector<TI>>& table, ostream& os)
{
    os << "\nTabla Hash\n";
    os << "-------------------------\n";
    for (size_t i = 0; i < table.size(); ++i)
    {
        os << "Bucket " << i << " : ";
        for (auto x : table[i])
            os << x << " ";
        os << "(" << table[i].size() << " elementos)";
        os << endl;
    }
}

// Obtiene la frecuencia empírica de buckets
vector<TI> histogram(const vector<vector<TI>>& table)
{
    size_t maxSize = 0;

    for (auto& b : table)
        maxSize = max(maxSize, b.size());

    vector<TI> hist(maxSize + 1, 0);

    for (auto& b : table)
        hist[b.size()]++;

    return hist;
}

void DemoHash(){

    cout << "\n=====================================\n";
    cout << " EXPERIMENTO 1\n";
    cout << " Colisiones provocadas\n";
    cout << "=====================================\n";

    const TI buckets = 5;
    vector<vector<TI>> table(buckets);

    // Valores escogidos para que TODOS colisionen
    vector<TI> keys = {0,5,10,15,20,25,30,35};

    cout << "\nInsertando claves:\n";
    for (TI k : keys)
    {
        TI index = k % buckets;
        cout<< setw(3) << k << " -> bucket " << index << endl;
        table[index].push_back(k);
    }

    printTable(table, cout);

    cout << "\nObservacion:\n";
    cout << "Todos los elementos terminan en el bucket 0.\n";
    cout << "La funcion hash NO distribuye uniformemente.\n";
    cout << "No podemos esperar una distribucion de Poisson.\n";

    cout << "\n\n=====================================\n";
    cout << " EXPERIMENTO 2\n";
    cout << " Distribucion uniforme\n";
    cout << "=====================================\n";

    const TI M = 1000;   // buckets
    const TI N = 1000;   // elementos

    vector<vector<TI>> hashTable(M);
    mt19937 rng(2026);

    uniform_int_distribution<TI> dist(0, 100000000);

    //---------------------------------------------------------
    // Inserciones aleatorias
    //---------------------------------------------------------

    for (size_t i = 0; i < N; ++i)
    {
        TI value = dist(rng);
        TI bucket = value % M;
        hashTable[bucket].push_back(value);
    }

    //---------------------------------------------------------
    // Histograma empirico
    //---------------------------------------------------------

    auto hist = histogram(hashTable);
    TD lambda = TD(N) / M;

    cout << fixed << setprecision(6);

    cout << "\nNumero de buckets = " << M << endl;
    cout << "Numero de elementos = " << N << endl;
    cout << "Lambda = n/m = " << lambda << endl;

    cout << "\nContenido de los primeros 20 buckets\n";

    for (size_t i = 0; i < 20; ++i)
    {
        cout << "Bucket " << i << " -> " << hashTable[i].size() << " elementos\n";
    }

    cout << "\n=============================================\n";
    cout << "Distribucion Empirica vs Poisson\n";
    cout << "=============================================\n";

    cout << setw(10) << "k" << setw(15) << "Empirica" << setw(15) << "Poisson" << endl;

    for (size_t k = 0; k < hist.size(); ++k)
    {
        TD empirical = TD(hist[k]) / M;
        cout << setw(10) << k << setw(15) << empirical << setw(15) << poisson(k, lambda) << endl;
    }

    cout << "\nComentarios\n";
    cout << "-----------\n";
    cout << "Cada bucket representa una observacion.\n";
    cout << "La teoria dice que el numero de elementos\n";
    cout << "por bucket sigue aproximadamente una Poisson\n";
    cout << "de media lambda=n/m.\n";
}