#include"./sequential.h"

template <typename ObjType, typename MembType>
SequentialFile<ObjType, MembType>::SequentialFile(string name_file, MembType primary_key) {
    this->name_file = name_file + ".dat";
    this->aux_name = name_file + "_aux.dat";
    this->primary_key = primary_key;
    ofstream (this->name_file);
    ofstream (this->aux_name);
}

template <typename ObjType, typename MembType>
void SequentialFile<ObjType, MembType>::joinFiles() {
    ObjType current;
    fstream file, aux_file, temp_file;
    file.open(this->name_file, ios::in | ios::binary);
    aux_file.open(this->aux_name, ios::in | ios::binary);
    temp_file.open("temp.dat", ios::out | ios::binary);

    long int next_record = 1;
    file >> current;

    Next temp = current.next;

    current.next.next_num = next_record;
    current.next.file = 'd';
    temp_file << current;

    while(temp.next_num != -1) {
        ++next_record;
        if(temp.file == 'd') {
            file.seekg(temp.next_num * (sizeof(ObjType) + 1));
            file >> current;
        } else {
            aux_file.seekg(temp.next_num * (sizeof(ObjType) + 1));
            aux_file >> current;
        }

        temp.next_num = current.next.next_num;
        temp.file = current.next.file;

        current.next.next_num = next_record;
        current.next.file = 'd';

        temp_file << current;
    }

    temp_file.seekg(temp_file.tellg() - (sizeof(ObjType) + 1));
    current.next.next_num = -1;
    temp_file << current;

    file.close();
    aux_file.close();
    temp_file.close();
    remove(this->name_file.c_str());
    remove(this->aux_name.c_str());
    rename("temp.dat", this->name_file.c_str());
    ofstream (this->aux_name);
}

template <typename ObjType, typename MembType>
void SequentialFile<ObjType, MembType>::insertAll(vector<ObjType> records) {
    fstream file;
    file.open(this->name_file, ios::in | ios::out | ios::binary);

    if(file.is_open()) {
        sort(records.begin(), records.end(), [this](const ObjType& r1, const ObjType& r2) {
            return strcmp(r1.*primary_key, r2.*primary_key) < 0;
        });

        file.seekg(0, ios::end);

        long int next_record = 1;

        for(auto it = begin(records); it != end(records); ++it) {
            (*it).next.next_num = next_record;

            if(it == end(records) - 1) {
                (*it).next.next_num = -1;
            }

            file << (*it);
            ++next_record;
        }

        file.close();
    } else {
        cerr << "Can't open file " << this->name_file << endl;
    }
}

template <typename ObjType, typename MembType>
ObjType SequentialFile<ObjType, MembType>::search(const string& key) {
    ObjType result;

    fstream file;
    file.open(this->name_file, ios::in | ios::binary);

    if(!file.is_open()) {
        cerr << "Can't open file " << this->name_file << endl;
        throw "Can't open file";
    }

    bool find = false;

    file.seekg(0, ios::end);
    long unsigned int size = file.tellg() / (sizeof(ObjType) + 1);

    long unsigned int position = binarySearch<ObjType>(file, size, this->primary_key, key, find);

    file.seekg(position * (sizeof(ObjType) + 1));
    file >> result;

    file.close();

    if(!find && result.next.file == 'd') {
        cerr << "Record " << key << " doesn't exists in " << this->name_file << endl;
        throw "Record doesn't exists";
    }

    if(find) {
        return result;
    } else {
        fstream aux_file;
        aux_file.open(this->aux_name, ios::in | ios::binary);

        if(!aux_file.is_open()) {
            cerr << "Can't open file " << this->aux_name << endl;
            throw "Can't open file";
        }

        linearSearch(aux_file, this->primary_key, key, result, find);

        aux_file.close();

        if(!find) {
            cerr << "Record " << key << " doesn't exists in " << this->aux_name << endl;
            throw "Record doesn't exists";
        }
        
        return result;
    } 
}

template <typename ObjType, typename MembType>
vector<ObjType> SequentialFile<ObjType, MembType>::search(const string& begin, const string& end) {
    vector<ObjType> records;

    ObjType result;

    fstream file, aux_file;
    file.open(this->name_file, ios::in | ios::binary);
    aux_file.open(this->aux_name, ios::in | ios::binary);

    if(file.is_open() && aux_file.is_open()) {
        bool find = false;

        file.seekg(0, ios::end);
        long unsigned int size = file.tellg() / (sizeof(ObjType) + 1);

        long unsigned int position = binarySearch<ObjType>(file, size, this->primary_key, begin, find);

        if(!find && result.next.file == 'd') {
            cerr << "Record " << begin << " doesn't exists in " << this->name_file << endl;
            throw "Can't find";
        }

        if(find) {
            file.seekg(position * (sizeof(ObjType) + 1));
            file >> result;
        } else {
            bool find = false;

            linearSearch(aux_file, this->primary_key, begin, result, find);

            if(!find) {
                cerr << "Record " << begin << " doesn't exists in " << this->aux_name << endl;
                throw "Record doesn't exists";
            }
        }

        records.push_back(result);

        while(result.next.next_num != -1 && result.*primary_key != end) {
            if(result.next.file == 'd') {
                file.seekg(result.next.next_num * (sizeof(ObjType) + 1));
                file >> result;
            } else {
                aux_file.seekg(result.next.next_num * (sizeof(ObjType) + 1));
                aux_file >> result;
            }

            records.push_back(result);
        }


        file.close();
        aux_file.close();
    } else {
        throw "Can't open file";
    }
    
    return records;
}

template <typename ObjType, typename MembType>
void SequentialFile<ObjType, MembType>::add(ObjType registro) {
    fstream file;
    file.open(this->name_file, ios::in | ios::out | ios::binary);

    if(file.is_open()) {
        bool find = false;

        file.seekg(0, ios::end);
        long unsigned int size = file.tellg() / (sizeof(ObjType) + 1);

        long unsigned int position = binarySearch<ObjType>(file, size, this->primary_key, registro.*primary_key, find);

        if(!find) {
            ObjType current;
            file.seekg(position * (sizeof(ObjType) + 1));
            file >> current;

            fstream aux_file;
            aux_file.open(this->aux_name, ios::in | ios::out | ios::binary);

            if(aux_file.is_open()) {
                long unsigned int aux_position = 0;
                Next next_current;

                if(current.next.file == 'a') {
                    aux_position = linearSearch(aux_file, this->primary_key, registro.*primary_key, current, find);

                    if(find) {
                        throw "Record already exists";
                    }

                    next_current = current.next; 

                    aux_file.seekg(0, ios::end);
                    current.next.next_num = (aux_file.tellg() / (sizeof(ObjType) + 1));
                    current.next.file = 'a';
                    
                    aux_file.seekg(aux_position * (sizeof(ObjType) + 1));
                    aux_file << current;
                } else {
                    next_current = current.next; 

                    aux_file.seekg(0, ios::end);
                    current.next.next_num = aux_file.tellg() / (sizeof(ObjType) + 1);
                    current.next.file = 'a';

                    file.seekg(position * (sizeof(ObjType) + 1));
                    file << current;
                }

                aux_file.seekg(0, ios::end);
                registro.next = next_current;
                aux_file << registro;

                long unsigned int aux_size = aux_file.tellg() / (sizeof(ObjType) + 1);

                aux_file.close();

                if(aux_size == 5) {
                    joinFiles();
                }
            } else {
                cerr << "Can't open file " << this->aux_name << endl;
            }
        } else {
            cerr << "Record already exists\n";
        }

        file.close();
    } else {
        cerr << "Can't open file " << this->name_file << endl;
    }
}

template class SequentialFile<Registro, char (Registro::*)[20]>;
template class SequentialFile<Registro, char (Registro::*)[5]>;
template class SequentialFile<Registro, char (Registro::*)[15]>;
