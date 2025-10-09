#ifndef SUDO_LIB_H
#define SUDO_LIB_H

#include <string>
#include "selector_codificacion.h"

// Crea el archivo sudo.txt con el administrador inicial
void crearArchivoAdministrador();

// Valida si el usuario ingresado es el administrador
bool validarAdministrador(const std::string& cedula, const std::string& clave);

// Valida si el usuario ingresado es un usuario normal y devuelve su saldo
bool validarUsuario(const std::string& cedula, const std::string& clave, double& saldo);

// Crea un nuevo usuario con su cedula, clave y saldo inicial
void crearUsuario(const std::string& cedula, const std::string& clave, double saldo);

// Actualiza el saldo de un usuario existente en el archivo
void actualizarSaldoEnArchivo(const std::string& cedula, double nuevoSaldo);

// Muestra el menu del usuario donde puede consultar saldo o retirar dinero
void mostrarMenuUsuarioConTransacciones(const std::string& cedula, double saldo, const EleccionCodificacion& eleccion);

// Funcion principal del flujo de autenticacion y administracion
int creadorprincipal();

#endif
