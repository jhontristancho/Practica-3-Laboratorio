#include "selector_codificacion.h"
#include <iostream>
using namespace std;

EleccionCodificacion seleccionarTipoYMetodo() {
    EleccionCodificacion eleccion;
    eleccion.tipo = 0;
    eleccion.metodo = 0;

    cout << "elige el tipo de dato: " << endl;
    cout << "1. usar string"<<endl;
    cout << "2. usar arreglo char"<< endl;
    cout << "solo puedes ingresar 1 o 2: ";
    cin >> eleccion.tipo;
    if (eleccion.tipo != 1 && eleccion.tipo != 2) {
        cout << "sabes que no se puede ingresar eso, voy a usar string";
        eleccion.tipo = 1;
    }
    cout << "ahora seleccione el metodo en el que quiera codificar"<< endl;
    cout << "1. metodo 1"<< endl;
    cout << "2. metodo 2"<<endl;
    cout << "solo 1 o 2: ";
    cin >> eleccion.metodo;

    if (eleccion.metodo != 1 && eleccion.metodo != 2) {
        cout << "sabes que no se puede, voy a usar el metodo 1";
        eleccion.metodo = 1;
    }

    cout << "se va usar: ";
    if (eleccion.tipo == 1)
        cout << "estructura string";
    else
        cout << "estructura char";

    cout << "metodo de codificacion " << eleccion.metodo << "\n";

    return eleccion;
}
