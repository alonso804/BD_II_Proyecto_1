#include"./next.h"

Next::Next() {
    this->position = 0;
    this->file = 'd';
}

Next& Next::operator = (const Next& record) {
    this->position = record.position;
    this->file = record.file;

    return *this;
}
