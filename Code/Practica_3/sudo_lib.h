#ifndef SUDO_LIB_H
#define SUDO_LIB_H

#include <fstream>
#include <string>

// Crea el archivo sudo.txt con el administrador inicial (si no existe)
void crearArchivoAdministrador();

// Valida credenciales de administrador
bool validarAdministrador(const std::string& cedula, const std::string& clave);

// Crea un usuario en sudo.txt (cedula, clave, saldo)
void crearUsuario(const std::string& cedula, const std::string& clave, double saldo);
int creadorprincipal();

#endif
