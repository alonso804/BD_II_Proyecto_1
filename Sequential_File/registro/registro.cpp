#include"./registro.h"

Registro& Registro::operator = (const Registro& record) {
    strncpy (this->codigo, record.codigo, sizeof(this->codigo));
    strncpy (this->nombre, record.nombre, sizeof(this->nombre));
    strncpy (this->carrera, record.carrera, sizeof(this->carrera));
    this->ciclo = record.ciclo;
    this->next = record.next;

    return *this;
}

void Registro::print() {
    cout << "Codigo: " << codigo << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Carrera: " << carrera << endl;
    cout << "Ciclo: " << ciclo << endl;
    cout << "Next: (" << next.next_num << ", " << next.file << ")" << endl;
}

ostream& operator << (ostream& stream, const Registro& record) {
    stream.write((char*)&record, sizeof(Registro));
    stream << '\n';
    stream << flush;

    return stream;
}

istream& operator >> (istream& stream, const Registro& record) {
    stream.read((char*)&record, sizeof(Registro));
    stream.get();

    return stream;
}
