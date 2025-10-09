#include "bin_manager.h"
#include "metodos_codificacion.h" // Para la implementación con std::string
#include "codificacionchar.h"     // Para la implementación con char[]

#include <fstream>
#include <iostream>
#include <string>
#include <cstring> // Para strcpy
#include <ctime>   // para generar semilla aleatoria
#include <cstdio>  // remove, rename

using namespace std;

// --- Funciones estáticas (privadas para este archivo) ---

// Convierte texto ASCII a bits binarios (string)
static string textoABinario(const string &texto) {
    string binario;
    for (char c : texto) {
        for (int i = 7; i >= 0; --i) {
            binario += ((c >> i) & 1) ? '1' : '0';
        }
    }
    return binario;
}

// Divide una línea del archivo sudo.txt en "cedula,clave,saldo"
static void dividirLineaSudo(const string &linea, string &cedula, string &clave, double &saldo) {
    size_t pos1 = linea.find(',');
    size_t pos2 = linea.find(',', pos1 + 1);

    if (pos1 == string::npos || pos2 == string::npos)
        throw "Error: formato inválido en sudo.txt (faltan comas).";

    cedula = linea.substr(0, pos1);
    clave = linea.substr(pos1 + 1, pos2 - pos1 - 1);
    string saldoTexto = linea.substr(pos2 + 1);

    try {
        saldo = stod(saldoTexto);
    } catch (...) {
        saldo = 0.0;
    }
}

// Aplica el método de codificación según la elección del usuario
static string aplicarCodificacion(const EleccionCodificacion &eleccion, const string &texto) {
    string binario = textoABinario(texto);
    string resultado;
    // Generamos una semilla basada en el tiempo, pero la hacemos un entero simple.
    // Usamos rand() para mayor variabilidad si se llama rápido.
    srand(time(nullptr));
    int semilla = (rand() % 8) + 2; // Semilla entre 2 y 9, como en el ejemplo.

    if (eleccion.tipo == 1) {
        // --- Implementación con std::string ---
        unsigned int n_semilla = semilla; // La función de string la espera como unsigned
        if (eleccion.metodo == 1)
            resultado = metodoCodificacion1(binario, n_semilla);
        else
            resultado = metodoCodificacion2(binario, n_semilla);

    } else {
        // --- Implementación con char[] (Sin STL Vector) ---
        int totalBits = binario.length();
        if (totalBits == 0) return "";

        // 1. Reservar memoria dinámica para los arreglos de C
        char* bits_entrada = new char[totalBits + 1];
        char* bits_salida = new char[totalBits + 1];

        // 2. Copiar el contenido del string al arreglo de char
        strcpy(bits_entrada, binario.c_str());

        // 3. Llamar a la función correcta de codificacionchar.cpp
        if (eleccion.metodo == 1) {
            metodo1(bits_entrada, bits_salida, totalBits, semilla);
        } else {
            metodo2(bits_entrada, bits_salida, totalBits, semilla);
        }

        // 4. Asegurarse de que el resultado sea una cadena de C válida
        bits_salida[totalBits] = '\0';

        // 5. Convertir el arreglo de char de vuelta a un std::string
        resultado = string(bits_salida);

        // 6. ¡Muy importante! Liberar la memoria reservada
        delete[] bits_entrada;
        delete[] bits_salida;
    }

    return resultado;
}


// --- Funciones Públicas ---

void crearBinUsuariosDesdeSudo(const EleccionCodificacion &eleccion) {
    try {
        ifstream archivoEntrada("sudo.txt");
        if (!archivoEntrada.is_open()) throw 1;

        ofstream archivoSalida("users.bin", ios::binary | ios::out | ios::trunc);
        if (!archivoSalida.is_open()) throw 1;

        string linea, cedula, clave;
        double saldo = 0.0;

        while (getline(archivoEntrada, linea)) {
            if (linea.empty()) continue;

            dividirLineaSudo(linea, cedula, clave, saldo);
            string claveEncriptada = aplicarCodificacion(eleccion, clave);

            // Guardar en el archivo binario
            unsigned int tam = claveEncriptada.length();
            archivoSalida.write(reinterpret_cast<const char*>(&tam), sizeof(tam));
            archivoSalida.write(claveEncriptada.c_str(), tam);
        }

        archivoEntrada.close();
        archivoSalida.close();
        cout << "Archivo users.bin creado/actualizado con las claves encriptadas.\n";

    } catch (const char* e) {
        cout << "Error: " << e << endl;
    } catch (int) {
        cout << "Error de lectura/escritura en archivos.\n";
    }
}

void registrarTransaccionEncriptada(const EleccionCodificacion &eleccion, const string &trxDescripcion) {
    try {
        if (trxDescripcion.empty()) throw "Descripción de transacción vacía.";

        ofstream archivoTransacciones("transactions.bin", ios::binary | ios::out | ios::app);
        if (!archivoTransacciones.is_open()) throw 1;

        string trxEncriptada = aplicarCodificacion(eleccion, trxDescripcion);

        unsigned int tam = trxEncriptada.length();
        archivoTransacciones.write(reinterpret_cast<const char*>(&tam), sizeof(tam));
        archivoTransacciones.write(trxEncriptada.c_str(), tam);
        archivoTransacciones.close();

        cout << "Transacción encriptada registrada correctamente.\n";

    } catch (const char* e) {
        cout << "Error: " << e << endl;
    } catch (int) {
        cout << "Error de E/S al registrar transacción.\n";
    }
}

void actualizarSaldoUsuarioEnSudo(const string &cedula, double nuevoSaldo) {
    try {
        if (cedula.empty()) throw "Error: cédula vacía.";

        ifstream archivoEntrada("sudo.txt");
        if (!archivoEntrada.is_open()) throw 1;

        // Usamos un archivo temporal para reescribir la información
        ofstream archivoTemporal("sudo_temp.txt", ios::out | ios::trunc);
        if (!archivoTemporal.is_open()) throw 1;

        string linea, cedulaActual, claveActual;
        double saldoActual = 0.0;
        bool encontrado = false;

        while (getline(archivoEntrada, linea)) {
            if (linea.empty()) {
                archivoTemporal << endl;
                continue;
            }

            dividirLineaSudo(linea, cedulaActual, claveActual, saldoActual);

            if (cedulaActual == cedula) {
                archivoTemporal << cedulaActual << "," << claveActual << "," << nuevoSaldo << endl;
                encontrado = true;
            } else {
                archivoTemporal << linea << endl; // Reescribir la línea original
            }
        }

        archivoEntrada.close();
        archivoTemporal.close();

        if (!encontrado) {
            remove("sudo_temp.txt");
            throw "Usuario no encontrado en sudo.txt.";
        }

        // Reemplazar el archivo original con el temporal
        remove("sudo.txt");
        rename("sudo_temp.txt", "sudo.txt");

        cout << "Saldo actualizado para el usuario " << cedula << ".\n";

    } catch (const char* e) {
        cout << "Error: " << e << endl;
    } catch (int) {
        cout << "Error de E/S al actualizar saldo.\n";
    }
}
