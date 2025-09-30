#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

/* Programa para escribir un numero o una palabra
 * en un archivo de texto*/
/* Programa para leer un archivo de texto
 * caracter por caracter o linea completa
 * y convertir un numero leido a una variable entera*/

int main()
{
    int numero1, numero2=0;
    char str[256];
    ifstream fin;         //Instancia de la clase iftream (lectura) para representar le archivo

    fin.open("archivo1.txt");  //Abrir el archivo para lectura
    if (!fin.is_open()) {
        std::cout << "Falla." << std::endl;
        return 1;  // Exit the program with an error code
    }

    //Leer por linea
    fin.getline(str,256);   //Obtener una linea
    stringstream val(str);  //Convertir a entero
    val >> numero1;         //Guardar el entero

    fin.seekg(0);           //Volver al inicio del archivo

    cout<<"Numero leido por linea: " <<numero1<<endl;

    //Leer por caracter
    while(fin.good()){       //Hasta el fin del archivo
        char temp=fin.get();     //Obtener el caracter
        if(fin.good()) numero2=numero2*10+temp-'0';  //Convertir el caracter a entero y acumular el valor
    }

    cout<<"Numero leido por caracter: " <<numero2<<endl;

    fin.close();    //Cerrar el archivo

}
