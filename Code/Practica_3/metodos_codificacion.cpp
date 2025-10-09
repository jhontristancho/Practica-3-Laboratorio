#include "metodos_codificacion.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

static char invertirBit(char bit) {
    return (bit == '0') ? '1' : '0';
}

static unsigned int generarSemilla() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    return static_cast<unsigned int>((std::rand() % 8) + 2); // 2..9
}

std::string metodoCodificacion1(const std::string& binario, unsigned int &n_out) {
    // Validaciones
    if (binario.empty()) throw "no ingresaste nada";
    for (char c : binario) if (c != '0' && c != '1') throw "solo 1 o 0";
    // determinar n
    unsigned int n = n_out;
    if (n == 0) n = generarSemilla();
    if (n == 0) throw "semilla invalida";
    n_out = n; // devolver semilla usada
    int len = static_cast<int>(binario.size());
    string salida;
    salida.resize(binario.size());

    // número de bloques (ceil)
    int numBloques = (len + static_cast<int>(n) - 1) / static_cast<int>(n);

    for (int b = 0; b < numBloques; ++b) {
        int start = b * static_cast<int>(n);
        int end = start + static_cast<int>(n);
        if (end > len) end = len;
        int blockSize = end - start;
        if (blockSize <= 0) continue;

        if (b == 0) {
            // invertir bits
            for (int i = start; i < end; ++i) salida[i] = invertirBit(binario[i]);
            continue;
        }
        // Contar 1s y 0s en el bloque anterior
        int prevStart = (b - 1) * static_cast<int>(n);
        int prevEnd = prevStart + static_cast<int>(n);
        if (prevEnd > len) prevEnd = len;
        int count1 = 0, count0 = 0;
        for (int i = prevStart; i < prevEnd; ++i) {
            if (binario[i] == '1') ++count1;
            else ++count0;
        }

        int step = 1;
        if (count1 == count0) step = 1;
        else if (count0 > count1) step = 2;
        else step = 3;
        for (int offset = 0; offset < blockSize; ++offset) {
            int idx = start + offset;
            if ((offset % step) == 0) salida[idx] = invertirBit(binario[idx]);
            else salida[idx] = binario[idx];
        }
    }

    return salida;
}

std::string metodoCodificacion2(const std::string& binario, unsigned int &n_out) {
    if (binario.empty()) throw "cadena vacia";
    for (char c : binario) if (c != '0' && c != '1') throw "solo 1 y 0";
    // determinar n
    unsigned int n = n_out;
    if (n == 0) n = generarSemilla();
    if (n == 0) throw "semilla mala";
    n_out = n; // devolver semilla usada
    int len = static_cast<int>(binario.size());
    string salida;
    salida.resize(binario.size());

    int numBloques = (len + static_cast<int>(n) - 1) / static_cast<int>(n);

    for (int b = 0; b < numBloques; ++b) {
        int start = b * static_cast<int>(n);
        int end = start + static_cast<int>(n);
        if (end > len) end = len;
        int blockSize = end - start;
        if (blockSize <= 0) continue;

        if (blockSize == 1) {
            salida[start] = binario[start];
            continue;
        }

        for (int offset = 0; offset < blockSize; ++offset) {
            int srcOffset = (offset - 1);
            if (srcOffset < 0) srcOffset += blockSize;
            int srcIdx = start + srcOffset;
            int destIdx = start + offset;
            salida[destIdx] = binario[srcIdx];
        }
    }

    return salida;
}
//sobrecarga para llamar sin la semilla
std::string metodoCodificacion1(const std::string& binario) {
    unsigned int n = 0;
    try {
        return metodoCodificacion1(binario, n);
    } catch (const char* msg) {
        cerr << msg << endl;
        return string();
    } catch (...) {
        cerr << "error desconocido" << endl;
        return string();
    }
}
std::string metodoCodificacion2(const std::string& binario) {
    unsigned int n = 0;
    try {
        return metodoCodificacion2(binario, n);
    } catch (const char* msg) {
        cerr << msg << endl;
        return string();
    } catch (...) {
        cerr << "error desconocido" << endl;
        return string();
    }
}
