#ifndef Functions_H
#define Functions_H

#include"../header.h"
#include"../record/index.h"

template <typename T>
size_t linear_search_index(fstream& file, const T& key, const size_t& entries_per_pages, const size_t& pos) {
    Index index;
    size_t i = 0;

    file.seekg(pos * (sizeof(Index) + 1));
    while(file >> index && index.primary_key < key && i < entries_per_pages) {
        ++i;
    }

    size_t position = index.pos;

    return position;
}

template <typename ObjType, typename MembType, typename T>
bool linear_search_leaf(fstream& file, fstream& aux_file, const T& key, MembType primary_key, const size_t& entries_leaf, const size_t& pos, pair<ObjType, char>& result) {
    file.seekg((pos * (sizeof(ObjType) + 1)) + (entries_leaf * (sizeof(ObjType) + 1)));

    ObjType last;
    file >> last;

    file.seekg(pos * (sizeof(ObjType) + 1));
    ObjType record;

    while(record.next.position != last.next.position && record.*primary_key < last.*primary_key) {
        if(record.next.file == 'd') {
            file.seekg(record.next.position * (sizeof(ObjType) + 1));
            file >> record;
        } else {
            aux_file.seekg(record.next.position * (sizeof(ObjType) + 1));
            aux_file >> record;
        }
    }

    if(record.*primary_key == key) {
        if(record.next.file == 'd') {
            result = make_pair(record, 'd');
        } else {
            result = make_pair(record, 'a');
        }
        return true;
    } else {
        return false;
    }
}

template <typename ObjType, typename MembType>
void indexer_leaf(fstream& current_file, fstream& prev_file, const size_t& size, const size_t& pos, MembType primary_key) {
    ObjType record;

    prev_file.seekg(pos * size);
    prev_file >> record;
    Index index (record.*primary_key, pos * size);

    current_file.seekg(0, ios::end);
    current_file << index;
}

inline void indexer(fstream& current_file, fstream& prev_file, const size_t& size, const size_t& pos) {
    Index index;

    prev_file.seekg(pos * size);
    prev_file >> index;
    index.pos = pos * size;

    current_file.seekg(0, ios::end);
    current_file << index;
}

#endif //Functions_H
