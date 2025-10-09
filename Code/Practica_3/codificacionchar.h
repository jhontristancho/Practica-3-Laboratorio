#ifndef CODIFICACIONCHAR_H
#define CODIFICACIONCHAR_H

void charABits(unsigned char c, char bits[8]) ;

unsigned char bitsAChar(char bits[8]);

void metodo1(const char bits[], char resultado[], int totalBits, int n);

void metodo2(const char bits[], char resultado[], int totalBits, int n);

void guardarBinario(const char bits[], int totalBits, const char* nombreSalida);

#endif

