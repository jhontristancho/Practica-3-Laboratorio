#include <iostream>
#include <string>
#include "sudo_lib.h"
#include "selector_codificacion.h"
#include "metodos_codificacion.h"
#include "codificacionchar.h"
#include "crypto_bin.h"

using namespace std;

int main() {
    try {
        cout << "\n=== Bienvenido al Sistema Bancario ===\n";

        // 1️⃣ Crear o verificar existencia del archivo sudo.txt
        crearArchivoAdministrador();

        // 2️⃣ Preguntar tipo de dato y método de codificación
        EleccionCodificacion eleccion = seleccionarTipoYMetodo();

        // 3️⃣ Generar archivo binario con claves encriptadas
        cout << "\nGenerando archivo binario de claves encriptadas...\n";
        generarBinUsuarios(eleccion);
        cout << "Archivo usuarios.bin actualizado correctamente.\n";

        // 4️⃣ Iniciar flujo de autenticación principal
        cout << "\nIniciando autenticación...\n";
        creadorprincipal(); // mantiene la lógica de login y creación de usuarios

        // Nota: mostrarMenuUsuarioConTransacciones() será llamada
        // dentro de creadorprincipal() después de validar que no es admin
    }
    catch (int) {
        cout << "[Error crítico]: No se pudo acceder o crear sudo.txt o los binarios.\n";
        return 1;
    }
    catch (bool) {
        cout << "[Error de validación]: Credenciales inválidas.\n";
        return 1;
    }
    catch (...) {
        cout << "[Error desconocido]: Ha ocurrido un fallo inesperado.\n";
        return 1;
    }

    cout << "\nEjecución finalizada correctamente.\n";
    return 0;
}
