#ifndef CRYPTO_BIN_H
#define CRYPTO_BIN_H

#include "selector_codificacion.h" // Para usar EleccionCodificacion

// --- Funciones principales ---

// Crea o actualiza el archivo binario con las claves encriptadas de usuarios y administrador
void generarBinUsuarios(const EleccionCodificacion& eleccion);

// Registra una transacción encriptada en su archivo binario correspondiente
void registrarBinTransaccion(const EleccionCodificacion& eleccion, const char* texto);

#endif
