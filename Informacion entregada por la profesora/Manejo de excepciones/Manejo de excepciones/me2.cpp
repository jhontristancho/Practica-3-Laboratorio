#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;


int main()
{
    ifstream fin;


    try {
        fin.open("nada.txt");


        cout<<"Antes del error"<<endl;
        if(!fin.is_open()){
            throw '1';
        }
        cout<<"Despues del error"<<endl;


    }  catch (char c) {
        cout<<"Error numero "<<c<<endl;
        if(c=='1') cout<<"Error en el archivo de lectura"<<endl;
    }
}
