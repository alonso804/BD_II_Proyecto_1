#ifndef Next_H
#define Next_H

struct Next {
    long int next_num;
    char file;

    Next();

    Next& operator = (const Next&);
};

#endif //Next_H
