#include "bin_manager.h"
#include "metodos_codificacion.h"
#include "codificacionchar.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstring> // Para strcpy
#include <ctime>   // para generar semilla aleatoria
#include <cstdio>  // remove, rename

using namespace std;

static string textoABinario(const string &texto) {
    string binario;
    for (char c : texto) {
        for (int i = 7; i >= 0; --i) {
            binario += ((c >> i) & 1) ? '1' : '0';
        }
    }
    return binario;
}
// divide en cedula clave y saldo
static void dividirLineaSudo(const string &linea, string &cedula, string &clave, double &saldo) {
    size_t pos1 = linea.find(',');
    size_t pos2 = linea.find(',', pos1 + 1);
    if (pos1 == string::npos || pos2 == string::npos)
        throw "hay un errro en el sudo.txt";
    cedula = linea.substr(0, pos1);
    clave = linea.substr(pos1 + 1, pos2 - pos1 - 1);
    string saldoTexto = linea.substr(pos2 + 1);
    try {
        saldo = stod(saldoTexto);
    } catch (...) {
        saldo = 0.0;
    }
}

static string aplicarCodificacion(const EleccionCodificacion &eleccion, const string &texto) {
    string binario = textoABinario(texto);
    string resultado;
    srand(time(nullptr));
    int semilla = (rand() % 8) + 2; // semilla entre 2 y 9
    if (eleccion.tipo == 1) {
        unsigned int n_semilla = semilla; // string recibe como argumento la semilla
        if (eleccion.metodo == 1)
            resultado = metodoCodificacion1(binario, n_semilla);
        else
            resultado = metodoCodificacion2(binario, n_semilla);

    } else {
        int totalBits = binario.length();
        if (totalBits == 0) return "";
        char* bits_entrada = new char[totalBits + 1];
        char* bits_salida = new char[totalBits + 1];
        strcpy(bits_entrada, binario.c_str());
        if (eleccion.metodo == 1) {
            metodo1(bits_entrada, bits_salida, totalBits, semilla);
        } else {
            metodo2(bits_entrada, bits_salida, totalBits, semilla);
        }
        bits_salida[totalBits] = '\0';
        // convertir el arreglo de char a string
        resultado = string(bits_salida);
        delete[] bits_entrada;
        delete[] bits_salida;
    }
    return resultado;
}
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
            unsigned int tam = claveEncriptada.length();
            archivoSalida.write(reinterpret_cast<const char*>(&tam), sizeof(tam));
            archivoSalida.write(claveEncriptada.c_str(), tam);
        }

        archivoEntrada.close();
        archivoSalida.close();
        cout << "se creo el .bin para los usuarios";

    } catch (const char* e) {
        cout << "hubo error" << e << endl;
    } catch (int) {
        cout << "erro en la escritura de los datos";
    }
}
void registrarTransaccionEncriptada(const EleccionCodificacion &eleccion, const string &trxDescripcion) {
    try {
        if (trxDescripcion.empty()) throw "transaccion vacia";

        ofstream archivoTransacciones("transactions.bin", ios::binary | ios::out | ios::app);
        if (!archivoTransacciones.is_open()) throw 1;
        string trxEncriptada = aplicarCodificacion(eleccion, trxDescripcion);
        unsigned int tam = trxEncriptada.length();
        archivoTransacciones.write(reinterpret_cast<const char*>(&tam), sizeof(tam));
        archivoTransacciones.write(trxEncriptada.c_str(), tam);
        archivoTransacciones.close();
        cout << "se codifico la transaccion";
    } catch (const char* e) {
        cout << "error" << e << endl;
    } catch (int) {
        cout << "no se pudo registrar la trasaccion, invalido";
    }
}
void actualizarSaldoUsuarioEnSudo(const string &cedula, double nuevoSaldo) {
    try {
        if (cedula.empty()) throw "error cedula vacia";

        ifstream archivoEntrada("sudo.txt");
        if (!archivoEntrada.is_open()) throw 1;
        // archivo temporal para escribir la informacion y que genere las validaciones
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
                archivoTemporal << linea << endl;
            }
        }

        archivoEntrada.close();
        archivoTemporal.close();

        if (!encontrado) {
            remove("sudo_temp.txt");
            throw "este usuario no se encontro en la base de datos";
        }
        // remplazar el temporal
        remove("sudo.txt");
        rename("sudo_temp.txt", "sudo.txt");

        cout << "saldo actualizado: " << cedula << ".\n";

    } catch (const char* e) {
        cout << "error: " << e << endl;
    } catch (int) {
        cout << "no se pudo actualizar el saldo ";
    }
}
