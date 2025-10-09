#include "sudo_lib.h"
#include "crypto_bin.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

void crearArchivoAdministrador() {
    ifstream fin("sudo.txt");
    if (fin.is_open()) {
        fin.close();
        return;
    }

    ofstream archivo("sudo.txt");
    if (!archivo.is_open()) {
        throw 1;
    }

    archivo << "1234567890,1234,0" << endl;
    archivo.close();
}

bool validarAdministrador(const string& cedula, const string& clave) {
    if (cedula == "1234567890" && clave == "1234") {
        return true;
    }
    throw false;
}

bool validarUsuario(const string& cedula, const string& clave, double& saldo) {
    ifstream fin("sudo.txt");
    if (!fin.is_open()) {
        throw 1;
    }

    string linea;
    bool primera = true;

    while (getline(fin, linea)) {
        if (primera) { primera = false; continue; }

        size_t pos1 = linea.find(',');
        size_t pos2 = linea.rfind(',');

        if (pos1 == string::npos || pos2 == string::npos || pos1 == pos2)
            continue;

        string ced = linea.substr(0, pos1);
        string cla = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        string sal = linea.substr(pos2 + 1);

        if (ced == cedula && cla == clave) {
            try {
                saldo = stod(sal);
            } catch (...) {
                throw "solo numeros";
            }
            fin.close();
            return true;
        }
    }

    fin.close();
    throw false;
}

void crearUsuario(const string& cedula, const string& clave, double saldo) {
    ofstream archivo("sudo.txt", ios::app);
    if (!archivo.is_open()) {
        throw 1;
    }
    archivo << cedula << "," << clave << "," << saldo << endl;
    archivo.close();
}

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

        if (ced == cedula)
            salida << ced << "," << cla << "," << nuevoSaldo << endl;
        else
            salida << linea << endl;
    }

    entrada.close();
    salida.close();

    remove("sudo.txt");
    rename("temp.txt", "sudo.txt");
}

void mostrarMenuUsuarioConTransacciones(const string& cedula, double saldo, const EleccionCodificacion& eleccion) {
    int opcion = 0;
    bool salir = false;

    while (!salir) {
        cout << "menu del usuario" << endl;
        cout << "cedula " << cedula << endl;
        cout << "1. Consultar saldo" << endl;
        cout << "2. retirar dinero" << endl;
        cout << "3. salir" << endl;
        cout << "seleccione alguna opcion: ";
        cin >> opcion;

        try {
            if (cin.fail()) throw "solo se valen numeros";

            if (opcion == 1) {
                if (saldo >= 1000) {
                    saldo -= 1000;
                    cout<< "su saldo es "<< saldo<<endl;
                    actualizarSaldoEnArchivo(cedula, saldo);
                    cout << "se le desconoto 1000 por consultar" << saldo << " COP" << endl;
                } else {
                    cout << "saldo insuficiente" << endl;
                }
            }
            else if (opcion == 2) {
                cout << "ingrese lo que va retirar. "<<endl;
                double monto;
                cin >> monto;

                if (cin.fail()) throw "se esperan numeros";
                if (monto <= 0) throw "monto negativo no vale";
                if (monto > saldo) throw "saldo insuficiente para retirar";
                saldo -= monto;
                actualizarSaldoEnArchivo(cedula, saldo);
                cout << "retiro exitoso" << saldo << " COP" << endl;
                string descripcion = "Retiro de " + to_string(monto) + " COP por usuario " + cedula;
                registrarBinTransaccion(eleccion, descripcion.c_str());
            }
            else if (opcion == 3) {
                salir = true;
            }
            else {
                throw "ingrese cosas coherentes";
            }
        }
        catch (const char* msg) {
            cout << "error desconocido " << msg << endl;
            cin.clear();
            string basura;
            getline(cin, basura);
        }
    }
}

int creadorprincipal() {
    try {
        crearArchivoAdministrador();

        string cedula, clave;
        cout << "Ingrese cedula: ";
        cin >> cedula;
        cout << "Ingrese clave: ";
        cin >> clave;

        try {
            if (validarAdministrador(cedula, clave)) {
                cout << "bienvenido admi" << endl;
                string nuevaCedula, nuevaClave;
                double nuevoSaldo = 0.0;
                cout << "ingrese cedula del nuevo usuario: ";
                cin >> nuevaCedula;
                cout << "ingrese clave de nuevo usuario: ";
                cin >> nuevaClave;
                cout << "ingrese saldo inicial: ";
                cin >> nuevoSaldo;

                if (cin.fail()) throw "debe ser numerico ";

                crearUsuario(nuevaCedula, nuevaClave, nuevoSaldo);
                cout << "usuario creado" << endl;
            }
        }
        catch (bool) {
            try {
                double saldo = 0.0;
                if (validarUsuario(cedula, clave, saldo)) {
                    cout << "Bienvenido usuario" << endl;
                    EleccionCodificacion eleccion;
                    eleccion.tipo = 1;
                    eleccion.metodo = 1;
                    mostrarMenuUsuarioConTransacciones(cedula, saldo, eleccion);
                }
            }
            catch (bool) {
                cout << "datos invalidos" << endl;
            }
        }
    }
    catch (const char* msg) {
        cout << "Error, intente de nuevo" << msg << endl;
        return 1;
    }
    catch (int) {
        cout << "Error en la base de datos" << endl;
        return 1;
    }
    catch (...) {
        cout << "Error, intente de nuevo" << endl;
        return 1;
    }

    return 0;
}
