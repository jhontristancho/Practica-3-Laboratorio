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
        crearArchivoAdministrador();
        // preguntar que metodo y que tipo
        EleccionCodificacion eleccion = seleccionarTipoYMetodo();
        // generar el archivo .bin. damos un cout como para ver que efectivamente se esta ejecutando
        cout << "se esta generando los .bin" << endl;
        generarBinUsuarios(eleccion);
        cout << ".bin creados"<<endl;
        creadorprincipal(); // logearse
        // mostrarMenuUsuarioConTransacciones() será llamada
        // dentro de creadorprincipal() despues de validar que no es admin
    }
    catch (int) {
        cout << "error, No se pudo acceder o crear sudo.txt";
        return 1;
    }
    catch (bool) {
        cout << "error, no se permiten esos caracteres"<<endl;
        return 1;
    }
    catch (...) {
        cout << "error, fallo inesperado";
        return 1;
    }

    cout << "finalizo exito.";
    return 0;
}
