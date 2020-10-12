#include"./isam.h"

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
void ISAM<ObjType, MembType, T, IndexAmount>::sortFile() {
    fstream file;
    file.open(this->name_file, ios::in | ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << "Can't open " << this->name_file << endl;
        return;
    }

    vector<ObjType> records;
    ObjType record;

    while(file >> record) {
        records.push_back(record);
    }

    this->size_file = records.size();

    sort(records.begin(), records.end(), [this](const ObjType& r1, const ObjType& r2) {
        return r1.*this->primary_key < r2.*this->primary_key;
    });

    file.seekg(0, ios::beg);

    for(size_t i = 1; i <= records.size(); ++i) {
        records[i].next.position = i;
        file << records[i];
    }

    file.close();
}

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
void ISAM<ObjType, MembType, T, IndexAmount>::createIndexes() {
    fstream file;
    file.open(this->name_file, ios::in | ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << RED << "Can't open " << this->name_file << RESET << endl;
        return;
    }

    fstream indexes[IndexAmount];

    for(size_t i = 0; i < IndexAmount; ++i) {
        indexes[i].open(this->index_name[i], ios::in | ios::out | ios::binary);

        if(!indexes[i].is_open()) {
            cerr << RED << "Can't open " << this->index_name[i] << RESET << endl;
            return;
        }
    }

    size_t leaf_entries = MAX_LEAF_ENTRIES;
    size_t leaf_pages = this->size_file / leaf_entries;

    this->entries_per_pages = pow(leaf_pages, (double)1/(double)IndexAmount);

    for(size_t i = 0; i < leaf_pages * leaf_entries; ++i) {
        if(i % (leaf_entries - 1) == 0) {
            indexer_leaf<ObjType, MembType, Index<T>>(indexes[IndexAmount - 1], file, sizeof(ObjType) + 1, i, this->primary_key);
        }
    }

    for(size_t i = IndexAmount - 2; i >= 0; --i) {
        leaf_entries = leaf_pages;
        leaf_pages /= entries_per_pages;

        for(size_t j = 0; j < leaf_pages * leaf_entries; ++j) {
            if(j % (leaf_entries - 1) == 0) {
                indexer<Index<T>>(indexes[i], indexes[i - 1], sizeof(index) + 1, j);
            }
        }
    }

    file.close();

    for(size_t i = 0; i < IndexAmount; ++i) {
        indexes[i].close();
    }
}

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
bool ISAM<ObjType, MembType, T, IndexAmount>::search_in_tree(fstream& file, fstream& aux_file, size_t& pos_prev, const T& key, pair<ObjType, char>& result) {
    fstream indexes[IndexAmount];

    for(size_t i = 0; i < IndexAmount; ++i) {
        indexes[i].open(this->index_name[i], ios::in | ios::out | ios::binary);

        if(!indexes[i].is_open()) {
            cerr << RED << "Can't open " << this->index_name[i] << RESET << endl;
            return false;
        }
    }

    size_t position = 0;

    for(size_t i = 0; i < IndexAmount; ++i) {
        if(i == 0) {
            while(indexes[i] >> this->index && index.primary_key < key);
            position = index.pos;
        } else {
            position = linear_search_index<Index<T>, T>(indexes[i], key, this->entries_per_pages, position);
        }
    }

    bool is_in_file = linear_search_leaf<ObjType, MembType, T>(file, aux_file, key, primary_key, MAX_LEAF_ENTRIES, position, result);


    file.close();
    aux_file.close();

    for(size_t i = 0; i < IndexAmount; ++i) {
        indexes[i].close();
    }

    return is_in_file;
}

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
ISAM<ObjType, MembType, T, IndexAmount>::ISAM(string name_file, MembType primary_key) {
    this->name_file = name_file + ".dat";
    this->name_aux = name_file + "_aux.dat";

    for(size_t i = 0; i < IndexAmount; ++i) {
        this->index_name[i] = "index" + to_string(i + 1) + ".dat";
    }

    this->primary_key = primary_key;

    sortFile();
    createIndexes();
}

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
void ISAM<ObjType, MembType, T, IndexAmount>::add(ObjType record) {
    fstream file;
    file.open(this->name_file, ios::in | ios::out | ios::binary);
    fstream aux_file;
    file.open(this->name_aux, ios::in | ios::out | ios::binary);

    if(!file.is_open() || !aux_file.is_open()) {
        cerr << RED << "Can't open " << this->name_file << RESET << endl;
        return;
    }

/*
 *    bool find = false;
 *
 *    file.seekg(0, ios::end);
 *    long unsigned int size = file.tellg() / (sizeof(ObjType) + 1);
 *
 *    //long unsigned int position = binarySearch<ObjType>(file, size, this->primary_key, registro.*primary_key, find);
 *    long unsigned int position = 1;
 *
 *    if(!find) {
 *        ObjType current;
 *        file.seekg(position * (sizeof(ObjType) + 1));
 *        file >> current;
 *
 *        fstream aux_file;
 *        aux_file.open(this->aux_name, ios::in | ios::out | ios::binary);
 *
 *        if(!aux_file.is_open()) {
 *            cerr << "Can't open file " << this->aux_name << endl;
 *            throw "Can't open file";
 *        }
 *
 *        long unsigned int aux_position = 0;
 *        Next next_current;
 *
 *        if(current.next.file == 'a') {
 *            aux_position = linearSearch(aux_file, this->primary_key, record.*primary_key, current, find);
 *
 *            if(find) {
 *                throw "Record already exists";
 *            }
 *
 *            next_current = current.next; 
 *
 *            aux_file.seekg(0, ios::end);
 *            current.next.position = (aux_file.tellg() / (sizeof(ObjType) + 1));
 *            current.next.file = 'a';
 *            
 *            aux_file.seekg(aux_position * (sizeof(ObjType) + 1));
 *            aux_file << current;
 *        } else {
 *            next_current = current.next; 
 *
 *            aux_file.seekg(0, ios::end);
 *            current.next.position = aux_file.tellg() / (sizeof(ObjType) + 1);
 *            current.next.file = 'a';
 *
 *            file.seekg(position * (sizeof(ObjType) + 1));
 *            file << current;
 *        }
 *
 *        aux_file.seekg(0, ios::end);
 *        record.next = next_current;
 *        aux_file << record;
 *
 *        //long unsigned int aux_size = aux_file.tellg() / (sizeof(ObjType) + 1);
 *
 *        aux_file.close();
 *    } else {
 *        cerr << "Record already exists\n";
 *    }
 *
 *    file.close();
 */

    //-Busqueda
}

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
ObjType ISAM<ObjType, MembType, T, IndexAmount>::search(const T& key) {
    ObjType p;
    return p;
}

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
vector<ObjType> ISAM<ObjType, MembType, T, IndexAmount>::search(const T& begin, const T& end) {
    throw new exception;
}

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
void ISAM<ObjType, MembType, T, IndexAmount>::remove(const T& key) {
    throw new exception;
}

template class ISAM<Aeropuerto, char(Aeropuerto::*)[5], string, 3>;
