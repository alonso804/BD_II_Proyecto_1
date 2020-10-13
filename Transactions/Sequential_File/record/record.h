#ifndef Record_H
#define Record_H

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
    /*
     *stream.write((char*)&record.id, sizeof(record.id));
     *stream.write((char*)&record.nombre, sizeof(record.nombre));
     *stream.write((char*)&record.ciudad, sizeof(record.ciudad));
     *stream.write((char*)&record.pais, sizeof(record.id));
     *stream.write((char*)&record.IATA, sizeof(record.IATA));
     *stream.write((char*)&record.ICAO, sizeof(record.ICAO));
     *stream.write((char*)&record.latitud, sizeof(record.latitud));
     *stream.write((char*)&record.longitud, sizeof(record.longitud));
     *stream.write((char*)&record.altitud, sizeof(record.altitud));
     *stream.write((char*)&record.Timezone, sizeof(record.Timezone));
     *stream.write((char*)&record.DST, sizeof(record.DST));
     *stream.write((char*)&record.TZ_database, sizeof(record.TZ_database));
     *stream.write((char*)&record.next.position, sizeof(record.next.position));
     *stream.write((char*)&record.next.file, sizeof(record.next.file));
     */
    stream << '\n';
    stream << flush;

    return stream;
}

istream& operator >> (istream& stream, const Aeropuerto& record) {
    stream.read((char*)&record, sizeof(record));
    /*
     *stream.read((char*)&record.id, sizeof(record.id));
     *stream.read((char*)&record.nombre, sizeof(record.nombre));
     *stream.read((char*)&record.ciudad, sizeof(record.ciudad));
     *stream.read((char*)&record.pais, sizeof(record.id));
     *stream.read((char*)&record.IATA, sizeof(record.IATA));
     *stream.read((char*)&record.ICAO, sizeof(record.ICAO));
     *stream.read((char*)&record.latitud, sizeof(record.latitud));
     *stream.read((char*)&record.longitud, sizeof(record.longitud));
     *stream.read((char*)&record.altitud, sizeof(record.altitud));
     *stream.read((char*)&record.Timezone, sizeof(record.Timezone));
     *stream.read((char*)&record.DST, sizeof(record.DST));
     *stream.read((char*)&record.TZ_database, sizeof(record.TZ_database));
     *stream.read((char*)&record.next.position, sizeof(record.next.position));
     *stream.read((char*)&record.next.file, sizeof(record.next.file));
     */
    stream.get();

    return stream;
}

#endif //Record_H

