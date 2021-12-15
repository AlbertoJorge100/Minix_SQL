#ifndef CIFRADO_H_INCLUDED
#define CIFRADO_H_INCLUDED
#include <iostream>
#include <cstring>
#include <string.h>
#define LONGITUD 5
#define RANGO 10
#define ASCII_ENDLN 10



using namespace std;

class Cifrado{
public:
    Cifrado(){}
    ~Cifrado(){}

    ///---------------------- estas son
    string cifrar(string palabra){
        //char a[2];
        //strcpy(a, letra.c_str());
        string respuesta;
        int ascii, aux_ascii;
        char l;


        for(int i=0;i<palabra.length();i++){
            ascii=(int)palabra[i];
            aux_ascii=ascii+RANGO;
            for(int j=0;j<LONGITUD;j++){
                if(aux_ascii==ASCII_ENDLN)
                    respuesta+="â†“";
                respuesta+=string(1,(char)(aux_ascii++));
            }
            respuesta+=string(1,l);
        }

        return respuesta;
    }
    string descifrar(string palabra){
        //char a[2];
        //strcpy(a, letra.c_str());
        string respuesta;
        int ascii, aux_ascii;

        for(int i=0;i<palabra.length();i+=LONGITUD+1){
            ascii=(int)palabra[i];
            aux_ascii=ascii-RANGO;
            respuesta+=(aux_ascii==254)?"\n":string(1,(char)(aux_ascii));
        }

        return respuesta;
    }
    ///---------------------- fin


    /*
    string cifrar(string palabra){
        //char a[2];
        //strcpy(a, letra.c_str());
        string respuesta;
        int ascii, aux_ascii;
        char l;


        for(int i=0;i<palabra.length();i++){
            ascii=(int)palabra[i];
            aux_ascii=ascii+RANGO;
            for(int j=0;j<LONGITUD;j++)
                respuesta+=string(1,(char)(aux_ascii++));
            respuesta+=string(1,l);
        }

        return respuesta;
    }


    string descifrar(string palabra){
        //char a[2];
        //strcpy(a, letra.c_str());

        string respuesta;
        int ascii, aux_ascii;

        for(int i=0;i<palabra.length();i+=LONGITUD+1){
            ascii=(int)palabra[i];
            aux_ascii=ascii-RANGO;
            respuesta+=string(1,(char)(aux_ascii));
        }

        return respuesta;
    }
    */
    /*if(aux_ascii==254)
                   respuesta+="\n";
               else
                   respuesta+=string(1,(char)(aux_ascii));*/
};


#endif // CIFRADO_H_INCLUDED
