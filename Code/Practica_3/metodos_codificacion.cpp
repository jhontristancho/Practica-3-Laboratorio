#include "metodos_codificacion.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;
char invertirBit(char bit) {
    return (bit == '0') ? '1' : '0';
}
string metodoCodificacion1(const string& binario) {
    try {
        if (binario.empty()) {
            throw "esta cadena esta vacia";
        }
        srand(time(0));//semilla
        int n = (rand() % 8) + 2; // n entre 2 y 9
        cout << "la semilla es:" << n << endl;//esto es solo para ver si esta funcionando la semilla
        string resultado;
        int longitud = binario.size();
        for (int i = 0; i < longitud; i += n) {
            string bloque = binario.substr(i, n);//para separar por bloques
            if (i == 0) {//primer bloque
                for (int j = 0; j < bloque.size(); j++) {
                    bloque[j] = invertirBit(bloque[j]);
                }
                resultado += bloque;
                continue;
            }
            string bloqueAnterior = resultado.substr(i - n, n);
            int conteo0 = 0, conteo1 = 0;
            for (int j = 0; j < bloqueAnterior.size(); j++) {
                if (bloqueAnterior[j] == '0') conteo0++;
                else conteo1++;
            }

            if (conteo0 == conteo1) {
                // invertir cada bit
                for (int j = 0; j < bloque.size(); j++) {
                    bloque[j] = invertirBit(bloque[j]);
                }
            } else if (conteo0 > conteo1) {
                // invertir cada 2 bits
                for (int j = 1; j < bloque.size(); j += 2) {
                    bloque[j] = invertirBit(bloque[j]);
                }
            } else {
                // invertir cada 3 bits
                for (int j = 2; j < bloque.size(); j += 3) {
                    bloque[j] = invertirBit(bloque[j]);
                }
            }

            resultado += bloque;
        }

        return resultado;
    }
    catch (const char* e) {
        cerr << e << endl;
        return "";
    }
    catch (...) {
        cerr << "error codificando por el m1" << endl;
        return "";
    }
}
