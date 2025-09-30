//Manejo de archivos en c++
#include <iostream>


//fstream incluye clases y mñetodos para leer y escribir archivos
#include <fstream>


using namespace std;


int main()
{
    // Instancia de clase ofstream
    ofstream fout;


    string line;


    //Por defecto borra el contenido, para agregar: fout.open("sample.txt", ios::app)
    fout.open("sample.txt");


    //Ejecutar un bucle si se abre el archivo
    while (fout) {


        // Leer de consola
        getline(cin, line);


        // -1 para salir
        if (line == "-1")
            break;


        // Escribir en el archivo
        fout << line << endl;
    }


    fout.close();


    // Instancia de la clase ifstream
    ifstream fin;




    fin.open("sample.txt");


    // Bucle hasta el fin de archivo
    while (getline(fin, line)) {


        // Imprima lo que lee en consola
        cout << line << endl;
    }




    fin.close();


    return 0;
}
