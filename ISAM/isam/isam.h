#ifndef ISAM_H
#define ISAM_H

#include"../header.h"

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
class ISAM {
    string name_file;
    string index_name[IndexAmount];
    MembType primary_key;

public:
    ISAM(string, MembType);

    void add(ObjType);

    ObjType search(const T&);

    void remove(const T&);

    //~ISAM();
};

// IndexAmount * IndexAmount - 1

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
ISAM<ObjType, MembType, T, IndexAmount>::ISAM(string name_file, MembType primary_key) {

}

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
void ISAM<ObjType, MembType, T, IndexAmount>::add(ObjType record) {

}

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
ObjType ISAM<ObjType, MembType, T, IndexAmount>::search(const T& key) {

}

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
void ISAM<ObjType, MembType, T, IndexAmount>::remove(const T& key) {

}
#endif //ISAM_H

