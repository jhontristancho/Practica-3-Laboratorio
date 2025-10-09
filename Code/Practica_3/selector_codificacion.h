#ifndef SELECTOR_CODIFICACION_H
#define SELECTOR_CODIFICACION_H

// Estructura para guardar la elección del usuario
struct EleccionCodificacion {
    int tipo;   // 1 = string, 2 = char
    int metodo; // 1 o 2
};

// Pregunta al usuario qué tipo de estructura y método usar.
// Devuelve una estructura con la elección.
EleccionCodificacion seleccionarTipoYMetodo();

#endif
