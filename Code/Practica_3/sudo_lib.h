#ifndef SUDO_LIB_H
#define SUDO_LIB_H

#include <string>
#include "selector_codificacion.h"

// Funciones existentes
void crearArchivoAdministrador();
bool validarAdministrador(const std::string& cedula, const std::string& clave);
bool validarUsuario(const std::string& cedula, const std::string& clave, double& saldo);
void crearUsuario(const std::string& cedula, const std::string& clave, double saldo);
int creadorprincipal();
void mostrarMenuUsuarioConTransacciones(const std::string& cedula, double saldo, const EleccionCodificacion& eleccion);
void actualizarSaldoEnArchivo(const std::string& cedula, double nuevoSaldo);

#endif
