#include "selector_codificacion.h"
#include <iostream>
using namespace std;

EleccionCodificacion seleccionarTipoYMetodo() {
    EleccionCodificacion eleccion;
    eleccion.tipo = 0;
    eleccion.metodo = 0;

    cout << "=== Selección de tipo de dato ===\n";
    cout << "1. Usar estructura string\n";
    cout << "2. Usar estructura char\n";
    cout << "Seleccione una opción (1 o 2): ";
    cin >> eleccion.tipo;

    if (eleccion.tipo != 1 && eleccion.tipo != 2) {
        cout << "Opción inválida. Se usará tipo string por defecto.\n";
        eleccion.tipo = 1;
    }

    cout << "\n=== Selección de método de codificación ===\n";
    cout << "1. Método de codificación 1\n";
    cout << "2. Método de codificación 2\n";
    cout << "Seleccione una opción (1 o 2): ";
    cin >> eleccion.metodo;

    if (eleccion.metodo != 1 && eleccion.metodo != 2) {
        cout << "Opción inválida. Se usará método 1 por defecto.\n";
        eleccion.metodo = 1;
    }

    cout << "\nHas seleccionado:\n";
    if (eleccion.tipo == 1)
        cout << "- Estructura: string\n";
    else
        cout << "- Estructura: char\n";

    cout << "- Método de codificación: " << eleccion.metodo << "\n";

    return eleccion;
}
