#include "sudo_lib.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// Crear archivo con administrador inicial
void crearArchivoAdministrador() {
    ifstream fin("sudo.txt");
    if (fin.is_open()) {
        fin.close();
        return;
    }

    fstream archivo_salida;
    archivo_salida.open("sudo.txt", ios::out);
    if (!archivo_salida.is_open()) {
        throw 1; // error crítico
    }

    // Línea 1: administrador
    archivo_salida << "1234567890,1234,0" << endl;
    archivo_salida.close();
}

// Validar administrador (solo la cédula y clave predefinidas)
bool validarAdministrador(const string& cedula, const string& clave) {
    if (cedula == "1234567890" && clave == "1234") {
        return true;
    }
    throw false; // acceso denegado
}

// Validar usuario normal (busca en el archivo, excepto la primera línea)
bool validarUsuario(const string& cedula, const string& clave, double& saldo) {
    ifstream fin("sudo.txt");
    if (!fin.is_open()) {
        throw 1; // error crítico
    }

    string linea;
    bool primera = true;
    while (getline(fin, linea)) {
        if (primera) { // saltar la primera línea (admin)
            primera = false;
            continue;
        }

        // formato esperado: cedula,clave,saldo
        size_t pos1 = linea.find(',');
        size_t pos2 = linea.rfind(',');

        if (pos1 == string::npos || pos2 == string::npos || pos1 == pos2) {
            continue; // línea mal formada
        }

        string ced = linea.substr(0, pos1);
        string cla = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        string sal = linea.substr(pos2 + 1);

        if (ced == cedula && cla == clave) {
            saldo = stod(sal); // convertir saldo a double
            fin.close();
            return true;
        }
    }

    fin.close();
    throw false; // no encontrado
}

// Crear usuario
void crearUsuario(const string& cedula, const string& clave, double saldo) {
    fstream archivo_salida;
    archivo_salida.open("sudo.txt", ios::app);
    if (!archivo_salida.is_open()) {
        throw 1; // error crítico
    }

    archivo_salida << cedula << "," << clave << "," << saldo << endl;
    archivo_salida.close();
}

// Función principal de pruebas
int creadorprincipal() {
    try {
        crearArchivoAdministrador();

        string cedula, clave;
        cout << "Ingrese cedula: ";
        cin >> cedula;
        cout << "Ingrese clave: ";
        cin >> clave;

        try {
            // Intentar como admin
            if (validarAdministrador(cedula, clave)) {
                cout << "✅ Acceso permitido. Bienvenido ADMINISTRADOR\n";

                // Crear usuario
                string cedulaUser, claveUser;
                double saldo;
                cout << "Ingrese cedula de nuevo usuario: ";
                cin >> cedulaUser;
                cout << "Ingrese clave de nuevo usuario: ";
                cin >> claveUser;
                cout << "Ingrese saldo inicial (COP): ";
                cin >> saldo;

                crearUsuario(cedulaUser, claveUser, saldo);
                cout << "Usuario creado correctamente\n";
            }
        }
        catch (bool) {
            // Si no es admin, intentar usuario normal
            try {
                double saldo = 0.0;
                if (validarUsuario(cedula, clave, saldo)) {
                    cout << "✅ Acceso permitido. Bienvenido USUARIO\n";
                    cout << "Su saldo actual es: " << saldo << " COP\n";
                }
            }
            catch (bool) {
                cout << "❌ Acceso denegado. Credenciales inválidas.\n";
            }
        }
    }
    catch (int) {
        cout << "[ERROR CRÍTICO]: No se pudo acceder al archivo 'sudo.txt'\n";
        return 1;
    }
    catch (...) {
        cout << "[ERROR FATAL]: Se capturó una excepción desconocida.\n";
        return 1;
    }

    return 0;
}
