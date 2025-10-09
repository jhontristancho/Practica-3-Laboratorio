#include "crypto_bin.h"
#include "metodos_codificacion.h"
#include "codificacionchar.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <cstring>

using namespace std;

void textoABits(const char* texto, char* bits, int& totalBits) {
    totalBits = 0;
    for (int i = 0; texto[i] != '\0'; ++i) {
        unsigned char c = texto[i];
        for (int b = 7; b >= 0; --b) {
            bits[totalBits++] = ((c >> b) & 1) ? '1' : '0';
        }
    }
}

void guardarBitsEnBinario(const char* bits, int totalBits, const char* nombreArchivo) {
    ofstream archivo(nombreArchivo, ios::binary | ios::app);
    if (!archivo.is_open()) throw 1;

    for (int i = 0; i < totalBits; i += 8) {
        char bloque[8];
        for (int j = 0; j < 8; ++j)
            bloque[j] = bits[i + j];
        unsigned char c = bitsAChar(bloque);
        archivo.write((char*)&c, 1);
    }
    archivo.close();
}

void generarBinUsuarios(const EleccionCodificacion& eleccion) {
    try {
        ifstream entrada("sudo.txt");
        if (!entrada.is_open()) throw 1;
        ofstream salida("usuarios.bin", ios::binary | ios::trunc);
        if (!salida.is_open()) throw 1;
        salida.close();
        char linea[256];
        while (entrada.getline(linea, sizeof(linea))) {
            if (strlen(linea) == 0) continue;
            // extrae la cedula y la clave
            char cedula[32] = {0}, clave[32] = {0};
            double saldo = 0.0;
            sscanf(linea, "%[^,],%[^,],%lf", cedula, clave, &saldo);
            char bits[1024];
            int totalBits = 0;
            textoABits(clave, bits, totalBits);
            // semilla aleatoria para codificar
            int n = (rand() % 5) + 4;
            char resultado[1024];
            memset(resultado, 0, sizeof(resultado));

            if (eleccion.tipo == 1) {
                std::string binario;
                for (int i = 0; i < totalBits; ++i) binario += bits[i];
                std::string codificado;
                if (eleccion.metodo == 1)
                    codificado = metodoCodificacion1(binario);
                else
                    codificado = metodoCodificacion2(binario);

                // Convertir string resultado a char[]
                for (int i = 0; i < (int)codificado.size(); ++i)
                    resultado[i] = codificado[i];
                totalBits = (int)codificado.size();
            } else {
                // Método basado en char (usa codificacionchar.cpp)
                if (eleccion.metodo == 1)
                    metodo1(bits, resultado, totalBits, n);
                else
                    metodo2(bits, resultado, totalBits, n);
            }

            guardarBitsEnBinario(resultado, totalBits, "usuarios.bin");
        }

        entrada.close();
    }
    catch (int) {
        cout << "error no se crearon los .bin";
        throw;
    }
    catch (...) {
        cout << "error desconocido, intente de nuevo";
        throw;
    }
}
void registrarBinTransaccion(const EleccionCodificacion& eleccion, const char* texto) {
    try {
        char bits[1024];
        int totalBits = 0;
        textoABits(texto, bits, totalBits);

        int n = (rand() % 5) + 4;
        char resultado[1024];
        memset(resultado, 0, sizeof(resultado));

        if (eleccion.tipo == 1) {
            std::string binario;
            for (int i = 0; i < totalBits; ++i) binario += bits[i];

            std::string codificado;
            if (eleccion.metodo == 1)
                codificado = metodoCodificacion1(binario);
            else
                codificado = metodoCodificacion2(binario);

            for (int i = 0; i < (int)codificado.size(); ++i)
                resultado[i] = codificado[i];
            totalBits = (int)codificado.size();
        } else {
            if (eleccion.metodo == 1)
                metodo1(bits, resultado, totalBits, n);
            else
                metodo2(bits, resultado, totalBits, n);
        }

        guardarBitsEnBinario(resultado, totalBits, "transacciones.bin");
    }
    catch (int) {
        cout << "error no se pudo crear el .bin";
        throw;
    }
    catch (...) {
        cout << "error desconocido intente de nuevo";
        throw;
    }
}
