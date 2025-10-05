#ifndef CODIFICACIONCHAR_H
#define CODIFICACIONCHAR_H

#include <vector>


void charABits(unsigned char c, char bits[8]);

unsigned char bitsAChar(char bits[8]);

std::vector<char> metodo1(const std::vector<char>& bits, int n);

std::vector<char> metodo2(const std::vector<char>& bits, int n);

void guardarBinario(const std::vector<char>& bits, const char* nombreSalida);

#endif

