#include"./next.h"

Next::Next() {
    this->next_num = 0;
    this->file = 'd';
}

Next& Next::operator = (const Next& record) {
    this->next_num = record.next_num;
    this->file = record.file;

    return *this;
}
