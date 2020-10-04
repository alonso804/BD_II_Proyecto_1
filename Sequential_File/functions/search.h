#ifndef Binary_Search_H
#define Binary_Search_H

#include"../header.h"
#include"../registro/registro.h"

template <typename ObjType, typename MembType, typename T>
long int binarySearch(fstream& file, const long unsigned int size, MembType memb, const T& key, bool& find) {
    long int mid;
    long int begin = 0;
    long int last = size - 1;

    ObjType obj;

    while(begin <= last) {
        mid = begin + (last - begin) / 2;

        file.seekg(mid * (sizeof(ObjType) + 1));

        file.read((char*) &obj, sizeof(ObjType));

        if(string(obj.*memb) == string(key)) {
            find = true;
            return mid;
        }

        if(string(obj.*memb) < string(key)) {
            begin = mid + 1;
        } else {
            last = mid - 1;
        }
    }

    if(string(key) > string(obj.*memb)) {
        return mid;
    } else {
        return mid - 1;
    }
}

template <typename ObjType, typename MembType, typename T>
long unsigned int linearSearch(fstream& file, MembType memb, const T& key, ObjType& obj, bool& find) {
    long unsigned int position;

    while(obj.next.next_num != -1 && obj.next.file != 'd' && obj.*memb != key) {
        file.seekg(obj.next.next_num * (sizeof(ObjType) + 1));
        position = file.tellg() / (sizeof(ObjType) + 1);
        file.read((char*) &obj, sizeof(ObjType));
    }

    if(obj.*memb == key) {
        find = true;
    }

    return position;
}

#endif //Binary_Search_H
