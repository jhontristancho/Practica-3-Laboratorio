#ifndef METODOS_CODIFICACION_H
#define METODOS_CODIFICACION_H

#include <string>

// Versiones nuevas: devuelven la cadena codificada y escriben en n_out la semilla usada.
// Si n_out == 0 la función generará una semilla y la pondrá en n_out.
std::string metodoCodificacion1(const std::string& binario, unsigned int &n_out);
std::string metodoCodificacion2(const std::string& binario, unsigned int &n_out);
std::string metodoCodificacion1(const std::string& binario);
std::string metodoCodificacion2(const std::string& binario);
#endif // METODOS_CODIFICACION_H
