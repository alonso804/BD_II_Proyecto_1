#ifndef Next_H
#define Next_H

#include"../../header.h"

struct Next {
    long int position;
    char file;

    Next();

    Next& operator = (const Next&);
};

Next::Next() {
    this->position = 0;
    this->file = 'd';
}

Next& Next::operator = (const Next& record) {
    this->position = record.position;
    this->file = record.file;

    return *this;
}

#endif //Next_H
