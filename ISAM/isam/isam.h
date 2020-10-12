#ifndef ISAM_H
#define ISAM_H

#include"../record/record.h"
#include"../record/index.h"
#include"../functions/functions.h"

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
class ISAM {
    string name_file;
    string name_aux;
    string index_name[IndexAmount];
    MembType primary_key;
    size_t size_file;
    size_t entries_per_pages;
    Index<T> index;

    void sortFile();

    void createIndexes();

    bool search_in_tree(fstream&, fstream&, size_t&, const T&, pair<ObjType, char>&);

public:
    ISAM(string, MembType);

    void add(ObjType);

    ObjType search(const T&);

    vector<ObjType> search(const T&, const T&);

    void remove(const T&);

    //~ISAM();
};

#endif //ISAM_H

