#ifndef Sequential_H
#define Sequential_H

#include"../registro/registro.h"
#include"../functions/search.h"

template <typename ObjType, typename MembType>
class SequentialFile {
    string name_file;
    string aux_name;
    MembType primary_key;

    void joinFiles();

public:
    SequentialFile(string, MembType);
    
    void insertAll(vector<ObjType>);

    ObjType search(const string& );

    vector<ObjType> search(const string&, const string&);

    void add(ObjType registro);
};

#endif //Sequential_H
