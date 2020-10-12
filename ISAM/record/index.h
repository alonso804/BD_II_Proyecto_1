#ifndef Index_H
#define Index_H

#include"../header.h"

struct Index {
    char primary_key[6];
    size_t pos;

    Index() = default;

    Index(char primary_key[6], size_t pos) {
        strncpy (this->primary_key, primary_key, sizeof(this->primary_key));
        this->pos = pos;
    }

    Index& operator = (const Index& index) {
        strncpy (this->primary_key, index.primary_key, sizeof(this->primary_key));
        this->pos = index.pos;

        return *this;
    }
};

ostream& operator << (ostream& stream, const Index& index) {
    stream.write((char*)&index, sizeof(index));
    stream << '\n';
    stream << flush;

    return stream;
}

istream& operator >> (istream& stream, const Index& index) {
    stream.read((char*)&index, sizeof(index));
    stream.get();

    return stream;
}

#endif //Index_H

