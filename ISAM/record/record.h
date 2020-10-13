#ifndef Record_H
#define Record_H

#include"../header.h"
#include"./next.h"

struct Aeropuerto {
    char id [6];
    char nombre [50];
    char ciudad [50];
    char pais [50];
    char IATA [4];
    char ICAO [5];
    char latitud [25];
    char longitud[25];
    char altitud[25];
    char Timezone [50];
    char DST [50];
    char TZ_database [50];
    char tipo [50];

    Next next;

    Aeropuerto() = default;

    Aeropuerto(string, string, string, string, string, string, string, string, string, string, string, string, string);

    Aeropuerto& operator = (const Aeropuerto&);

    void print();
};

Aeropuerto::Aeropuerto(string id, string nombre, string ciudad, string pais, string IATA, string ICAO, string latitud, string longitud, string altitud, string Timezone, string DST, string TZ_database, string tipo) {
    strncpy (this->id, id.c_str(), sizeof(this->id));
    strncpy (this->nombre, nombre.c_str(), sizeof(this->nombre));
    strncpy (this->ciudad, ciudad.c_str(), sizeof(this->ciudad));
    strncpy (this->pais, pais.c_str(), sizeof(this->pais));
    strncpy (this->IATA, IATA.c_str(), sizeof(this->IATA));
    strncpy (this->ICAO, ICAO.c_str(), sizeof(this->ICAO));
    strncpy (this->latitud, latitud.c_str(), sizeof(this->latitud));
    strncpy (this->longitud, longitud.c_str(), sizeof(this->longitud));
    strncpy (this->altitud, altitud.c_str(), sizeof(this->altitud));
    strncpy (this->Timezone, Timezone.c_str(), sizeof(this->Timezone));
    strncpy (this->DST, DST.c_str(), sizeof(this->DST));
    strncpy (this->TZ_database, TZ_database.c_str(), sizeof(this->TZ_database));
    strncpy (this->tipo, tipo.c_str(), sizeof(this->tipo));
    next.position = -1;
    next.file = 'd';
}

Aeropuerto& Aeropuerto::operator = (const Aeropuerto& record) {
    strncpy (this->id, record.id, sizeof(this->id));
    strncpy (this->nombre, record.nombre, sizeof(this->nombre));
    strncpy (this->ciudad, record.ciudad, sizeof(this->ciudad));
    strncpy (this->pais, record.pais, sizeof(this->pais));
    strncpy (this->IATA, record.IATA, sizeof(this->IATA));
    strncpy (this->ICAO, record.ICAO, sizeof(this->ICAO));
    strncpy (this->latitud, record.latitud, sizeof(this->latitud));
    strncpy (this->longitud, record.longitud, sizeof(this->longitud));
    strncpy (this->altitud, record.altitud, sizeof(this->altitud));
    strncpy (this->Timezone, record.Timezone, sizeof(this->Timezone));
    strncpy (this->DST, record.DST, sizeof(this->DST));
    strncpy (this->TZ_database, record.TZ_database, sizeof(this->TZ_database));
    strncpy (this->tipo, record.tipo, sizeof(this->tipo));
    this->next = record.next;

    return *this;
}

void Aeropuerto::print() {
    cout << "Id: " << id << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Ciudad: " << ciudad << endl;
    cout << "Next: (" << next.position << ", " << next.file << ")" << endl;
}

ostream& operator << (ostream& stream, const Aeropuerto& record) {
    stream.write((char*)&record, sizeof(record));
    stream << '\n';
    stream << flush;

    return stream;
}

istream& operator >> (istream& stream, const Aeropuerto& record) {
    stream.read((char*)&record, sizeof(record));
    stream.get();

    return stream;
}

#endif //Record_H

