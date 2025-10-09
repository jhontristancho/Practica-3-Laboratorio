#ifndef BIN_MANAGER_H
#define BIN_MANAGER_H

#include <string>
#include "selector_codificacion.h" // Necesario para la estructura EleccionCodificacion

/**
 * @brief Crea un archivo binario 'users.bin' encriptando las claves desde 'sudo.txt'.
 * @param eleccion La configuración de método y tipo de codificación seleccionada por el usuario.
 */
void crearBinUsuariosDesdeSudo(const EleccionCodificacion &eleccion);

/**
 * @brief Registra la descripción de una transacción de forma encriptada en 'transactions.bin'.
 * @param eleccion La configuración de método y tipo de codificación.
 * @param trxDescripcion El texto que describe la transacción.
 */
void registrarTransaccionEncriptada(const EleccionCodificacion &eleccion, const std::string &trxDescripcion);

/**
 * @brief Actualiza la línea de un usuario específico en 'sudo.txt' con su nuevo saldo.
 * @param cedula La cédula del usuario a actualizar.
 * @param nuevoSaldo El nuevo saldo a registrar.
 */
void actualizarSaldoUsuarioEnSudo(const std::string &cedula, double nuevoSaldo);

#endif // BIN_MANAGER_H
