#include "codificacionchar.h"
#include <fstream>
#include <iostream>
using namespace std;

void charABits(unsigned char c, char bits[8]) {
    for (int i = 7; i >= 0; i--) {
        bits[i] = (c & 1) + '0';  // '0' o '1'
        c >>= 1;
    }
}

unsigned char bitsAChar(char bits[8]) {
    unsigned char c = 0;
    for (int i = 0; i < 8; i++) {
        c = (c << 1) | (bits[i] - '0');
    }
    return c;
}

void metodo1(const char bits[], char resultado[], int totalBits, int n) {
    int bloques = totalBits / n;

    for (int b = 0; b < bloques; b++) {
        int start = b * n;
        int end   = start + n;

        if (b == 0) {
            // Primer bloque: invertir todos los bits
            for (int i = start; i < end; i++)
                resultado[i] = (bits[i] == '0') ? '1' : '0';
        } else {
            // Contar 0s y 1s del bloque anterior (sin codificar)
            int count1 = 0, count0 = 0;
            for (int i = (b-1)*n; i < b*n; i++) {
                if (bits[i] == '1') count1++;
                else count0++;
            }

            for (int i = start; i < end; i++) {
                if (count1 == count0) {
                    resultado[i] = (bits[i] == '0') ? '1' : '0';
                } else if (count0 > count1) {
                    if (((i - start + 1) % 2) == 0)
                        resultado[i] = (bits[i] == '0') ? '1' : '0';
                    else
                        resultado[i] = bits[i];
                } else {
                    if (((i - start + 1) % 3) == 0)
                        resultado[i] = (bits[i] == '0') ? '1' : '0';
                    else
                        resultado[i] = bits[i];
                }
            }
        }
    }
}

// ---------------- MÉTODO 2 ----------------
void metodo2(const char bits[], char resultado[], int totalBits, int n) {
    int bloques = totalBits / n;

    for (int b = 0; b < bloques; b++) {
        int start = b * n;
        int end   = start + n;

        for (int i = 0; i < n; i++) {
            if (i == 0)
                resultado[start] = bits[end - 1];
            else
                resultado[start + i] = bits[start + i - 1];
        }
    }
}

// ---------------- Guardar archivo binario ----------------
void guardarBinario(const char bits[], int totalBits, const char* nombreSalida) {
    ofstream salida(nombreSalida, ios::binary);

    for (int i = 0; i < totalBits; i += 8) {
        char byteBits[8];
        for (int j = 0; j < 8; j++) byteBits[j] = bits[i + j];
        unsigned char c = bitsAChar(byteBits);
        salida.write((char*)&c, 1);
    }

    salida.close();
}
