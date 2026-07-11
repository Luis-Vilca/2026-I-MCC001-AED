#ifndef __TYPES_H__
#define __TYPES_H__
/**
 * @file types.h
 * @brief Definición de alias de tipos utilizados en la biblioteca.
 *
 * Este archivo centraliza la definición de alias de tipos fundamentales
 * empleados por las diferentes estructuras de datos del proyecto,
 * facilitando su mantenimiento y portabilidad entre plataformas.
 *
 * @author Luis Vilca
 */

#include <string>
using namespace std;

// C style
// typedef int T;

// C++11 style
using TI = int;
using TD = double;
using TS = string;
using TB = bool;
using TL = long;
using TC = char;

// XT must be 32bit integer in Windows and 64bit in Linux
#if defined(_WIN32) || defined(_WIN64)
    using XT = int;
#else
    using XT = long;
#endif

using Ref = long;

#endif // __TYPES_H__