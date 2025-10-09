#include "sudo_lib.h"
#include "crypto_bin.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// ========================================================
// FUNCIONES BASE
// ========================================================

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
        throw 1;
    }
    archivo_salida << "1234567890,1234,0" << endl;
    archivo_salida.close();
}

// Validar administrador
bool validarAdministrador(const string& cedula, const string& clave) {
    if (cedula == "1234567890" && clave == "1234") {
        return true;
    }
    throw false;
}

// Validar usuario
bool validarUsuario(const string& cedula, const string& clave, double& saldo) {
    ifstream fin("sudo.txt");
    if (!fin.is_open()) {
        throw 1;
    }

    string linea;
    bool primera = true;
    while (getline(fin, linea)) {
        if (primera) {
            primera = false;
            continue; // omitir admin
        }

        size_t pos1 = linea.find(',');
        size_t pos2 = linea.rfind(',');

        if (pos1 == string::npos || pos2 == string::npos || pos1 == pos2) {
            continue;
        }
        string ced = linea.substr(0, pos1);
        string cla = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        string sal = linea.substr(pos2 + 1);

        if (ced == cedula && cla == clave) {
            saldo = stod(sal);
            fin.close();
            return true;
        }
    }

    fin.close();
    throw false;
}

// Crear nuevo usuario
void crearUsuario(const string& cedula, const string& clave, double saldo) {
    fstream archivo_salida;
    archivo_salida.open("sudo.txt", ios::app);
    if (!archivo_salida.is_open()) {
        throw 1;
    }
    archivo_salida << cedula << "," << clave << "," << saldo << endl;
    archivo_salida.close();
}

// ========================================================
// FUNCIONES DE ACTUALIZACIÓN Y TRANSACCIONES
// ========================================================

void actualizarSaldoEnArchivo(const string& cedula, double nuevoSaldo) {
    ifstream entrada("sudo.txt");
    if (!entrada.is_open()) throw 1;

    ofstream salida("temp.txt");
    if (!salida.is_open()) throw 1;

    string linea;
    while (getline(entrada, linea)) {
        if (linea.empty()) continue;

        size_t pos1 = linea.find(',');
        size_t pos2 = linea.rfind(',');

        if (pos1 == string::npos || pos2 == string::npos) {
            salida << linea << endl;
            continue;
        }

        string ced = linea.substr(0, pos1);
        string cla = linea.substr(pos1 + 1, pos2 - pos1 - 1);

        if (ced == cedula) {
            salida << ced << "," << cla << "," << nuevoSaldo << endl;
        } else {
            salida << linea << endl;
        }
    }

    entrada.close();
    salida.close();

    remove("sudo.txt");
    rename("temp.txt", "sudo.txt");
}

// ========================================================
// MENÚ DE TRANSACCIONES
// ========================================================

void mostrarMenuUsuarioConTransacciones(const string& cedula, double saldo, const EleccionCodificacion& eleccion) {
    int opcion = 0;
    double monto = 0.0;
    bool salir = false;

    while (!salir) {
        cout << "\n--- Menú de Usuario ---" << endl;
        cout << "Cédula: " << cedula << endl;
        cout << "Saldo actual: " << saldo << " COP" << endl;
        cout << "1. Consultar saldo" << endl;
        cout << "2. Retirar dinero" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            cout << "\nSu saldo actual es: " << saldo << " COP" << endl;
            break;

        case 2:
            cout << "Ingrese el monto a retirar: ";
            cin >> monto;
            if (monto <= 0) {
                cout << "Monto inválido.\n";
            } else if (monto > saldo) {
                cout << "Saldo insuficiente para realizar el retiro.\n";
            } else {
                saldo -= monto;
                actualizarSaldoEnArchivo(cedula, saldo);
                cout << "Retiro exitoso. Nuevo saldo: " << saldo << " COP\n";

                string descripcion = "Retiro de " + to_string(monto) + " COP por usuario " + cedula;
                registrarBinTransaccion(eleccion, descripcion.c_str());
            }
            break;

        case 3:
            salir = true;
            break;

        default:
            cout << "Opción inválida. Intente nuevamente.\n";
        }
    }
}

// ========================================================
// CONTROL PRINCIPAL (LOGIN Y ACCESO)
// ========================================================

int creadorprincipal() {
    try {
        crearArchivoAdministrador();

        string cedula, clave;
        cout << "\nIngrese cédula: ";
        cin >> cedula;
        cout << "Ingrese clave: ";
        cin >> clave;

        try {
            // Administrador
            if (validarAdministrador(cedula, clave)) {
                cout << "\nAcceso permitido. Bienvenido Administrador.\n";
                string nuevaCedula, nuevaClave;
                double nuevoSaldo;
                cout << "Ingrese cédula de nuevo usuario: ";
                cin >> nuevaCedula;
                cout << "Ingrese clave de nuevo usuario: ";
                cin >> nuevaClave;
                cout << "Ingrese saldo inicial: ";
                cin >> nuevoSaldo;
                crearUsuario(nuevaCedula, nuevaClave, nuevoSaldo);
                cout << "Usuario creado correctamente.\n";
            }
        }
        catch (bool) {
            // Usuario normal
            try {
                double saldo = 0.0;
                if (validarUsuario(cedula, clave, saldo)) {
                    cout << "\nAcceso permitido. Bienvenido Usuario.\n";
                    // 🔹 Menú de transacciones para usuario
                    EleccionCodificacion eleccion;
                    eleccion.tipo = 1;  // por defecto (string)
                    eleccion.metodo = 1;
                    mostrarMenuUsuarioConTransacciones(cedula, saldo, eleccion);
                }
            }
            catch (bool) {
                cout << "Acceso denegado. Credenciales inválidas.\n";
            }
        }
    }
    catch (int) {
        cout << "[Error] No se pudo acceder o crear sudo.txt.\n";
        return 1;
    }
    catch (...) {
        cout << "[Error] Se produjo una excepción desconocida.\n";
        return 1;
    }

    return 0;
}
