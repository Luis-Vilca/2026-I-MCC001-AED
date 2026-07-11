#ifndef __MACROS_H__
#define __MACROS_H__
/**
 * @file macros.h
 * @brief Definición de macros utilizadas para fines demostrativos.
 *
 * Este archivo contiene diversas macros empleadas para ilustrar el
 * funcionamiento del preprocesador de C++, además de declarar la función
 * que ejecuta la demostración correspondiente.
 *
 * @author Luis Vilca
 */

#include "types.h"
// Macros
#define X 5
// x = MULT(x+1, x+2)
#define SUMA(a, b) ((a) + (b))
#define MULT(a, b) ((a) * (b))
#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#define CUADRADO(a)((a) * (a))

#define GetVar(var) c##var

/**
 * @brief Ejecuta la demostración del uso de macros en C++.
 */
void DemoMacros();

#endif // __MACROS_H__