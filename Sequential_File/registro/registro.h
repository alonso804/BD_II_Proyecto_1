#ifndef Registro_H
#define Registro_H

#include"../header.h"
#include"./next.h"

struct Registro {
    char codigo[5];
    char nombre[20];
    char carrera[15];
    int ciclo;

    Next next;

    Registro() = default;

    Registro& operator = (const Registro&);

    void print();
};

ostream& operator << (ostream&, const Registro&);

istream& operator >> (istream&, const Registro&);

#endif //Registro_H
