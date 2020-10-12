#ifndef Index_H
#define Index_H

#include"../header.h"

template <typename T>
struct Index {
    T primary_key;
    size_t pos;
};

template <typename T>
inline ostream& operator << (ostream& stream, const Index<T>& index) {
    stream.write((char*)&index, sizeof(index));
    stream << '\n';
    stream << flush;

    return stream;
}

template <typename T>
inline istream& operator >> (istream& stream, const Index<T>& index) {
    stream.read((char*)&index, sizeof(index));
    stream.get();

    return stream;
}

#endif //Index_H

