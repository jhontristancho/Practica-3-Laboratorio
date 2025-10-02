#include "sudo_lib.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
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
        if (primera) {
            primera = false;
            continue;
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
            saldo = stod(sal); // convertir saldo a double
            fin.close();
            return true;
        }
    }

    fin.close();
    throw false; //por si no lo encuentra
}
void crearUsuario(const string& cedula, const string& clave, double saldo) {
    fstream archivo_salida;
    archivo_salida.open("sudo.txt", ios::app);
    if (!archivo_salida.is_open()) {
        throw 1;
    }
    archivo_salida << cedula << "," << clave << "," << saldo << endl;
    archivo_salida.close();
}
int creadorprincipal() {
    try {
        crearArchivoAdministrador();
        string cedula, clave;
        cout << "ingrese cedula:";
        cin >> cedula;
        cout << "ingrese clave:";
        cin >> clave;
        try {
            if (validarAdministrador(cedula, clave)) {
                cout << "acceso permitido, eres un admi";
                string cedulaUser, claveUser;
                double saldo;
                cout << "ingrese cedula de nuevo usuario: ";
                cin >> cedulaUser;
                cout << "ingrese clave de nuevo usuario: ";
                cin >> claveUser;
                cout << "ingrese saldo inicial: ";
                cin >> saldo;
                crearUsuario(cedulaUser, claveUser, saldo);
            }
        }
        catch (bool) {
            // esto se hace para validar que si no es admi, vaya a ver si es usuario
            try {
                double saldo = 0.0;
                if (validarUsuario(cedula, clave, saldo)) {
                    cout << "acesso permitido, eres usuario";
                    cout << "su saldo actual es: " << saldo ;
                }
            }
            catch (bool) {
                cout << "acesso invalido";
            }
        }
    }
    catch (int) {
        cout << "no se pudo acceder a sudo";
        return 1;
    }
    catch (...) {
        cout << "se ingreso algun comando invalido";
        return 1;
    }

    return 0;
}
