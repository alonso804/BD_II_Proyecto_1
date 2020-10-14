#ifndef Functions_H
#define Functions_H

#include"../header.h"
#include"../record/index.h"

template <typename T>
long int linear_search_index(fstream& file, const T& key, const size_t& entries_per_pages, const long int& pos) {
    Index index;
    size_t i = 0;

    file.seekg(pos * (sizeof(Index) + 1));
    file >> index;
    long int position = index.pos;

    while(file >> index && atoi(index.primary_key) < stoi(key) && i < entries_per_pages) {
        position = index.pos;
        ++i;
    }

    return position;
}

template <typename ObjType, typename MembType, typename T>
bool linear_search_leaf(fstream& file, fstream& aux_file, const T& key, MembType primary_key, const size_t& entries_leaf, long int& pos, pair<ObjType, char>& result) {
    ObjType record;

    file.seekg((pos + entries_leaf) * (sizeof(ObjType) + 1));

    ObjType last;
    file >> last;

    file.seekg(pos * (sizeof(ObjType) + 1));
    pos = file.tellg() / (sizeof(ObjType) + 1);
    file >> record;

    while(record.next.position < last.next.position && record.next.position >= (long int)-1 && atoi(record.*primary_key) < stoi(key) && atoi(record.*primary_key) < atoi(last.*primary_key)) {
        if(record.next.file == 'd') {
            file.seekg(record.next.position * (sizeof(ObjType) + 1));
            pos = file.tellg() / (sizeof(ObjType) + 1);
            file >> record;
        } else {
            aux_file.seekg(record.next.position * (sizeof(ObjType) + 1));
            pos = aux_file.tellg() / (sizeof(ObjType) + 1);
            aux_file >> record;
        }
    }

    result = (record.next.file == 'd') ? make_pair(record, 'd') : make_pair(record,'a');
    return (record.*primary_key == key) ? true : false;
}

template <typename ObjType, typename MembType>
void indexer_leaf(fstream& current_file, fstream& prev_file, const size_t& size, const long int& pos, MembType primary_key) {
    ObjType record;

    prev_file.seekg(pos * size);
    prev_file >> record;
    Index index (record.*primary_key, pos);

    current_file.seekg(0, ios::end);
    current_file << index;
}

inline void indexer(fstream& current_file, fstream& prev_file, const size_t& size, const long int& pos) {
    Index index;

    prev_file.seekg(pos * size);
    prev_file >> index;
    index.pos = pos;

    current_file.seekg(0, ios::end);
    current_file << index;
}

#endif //Functions_H
