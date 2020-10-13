#ifndef ISAM_H
#define ISAM_H

#include"../record/record.h"
#include"../functions/functions.h"

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
class ISAM {
    string name_file;
    string name_aux;
    string index_name[IndexAmount];
    MembType primary_key;
    size_t size_file;
    size_t entries_per_pages;
    pair<size_t, size_t> pages_entries[IndexAmount];
    Index index;

    void sortFile();

    void createIndexes();

    bool search_in_tree(fstream&, fstream&, const T&, long int&, pair<ObjType, char>&);

public:
    ISAM(string, MembType);

    void add(ObjType);

    ObjType search(const T&);

    vector<ObjType> search(const T&, const T&);

    ~ISAM();
};

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
ISAM<ObjType, MembType, T, IndexAmount>::ISAM(string name_file, MembType primary_key) {
    this->name_file = name_file + ".dat";
    this->name_aux = name_file + "_aux.dat";
    ofstream(this->name_aux);

    for(size_t i = 0; i < IndexAmount; ++i) {
        this->index_name[i] = "index_" + to_string(i + 1) + ".dat";
        ofstream(this->index_name[i]);
    }

    this->primary_key = primary_key;

    sortFile();
    createIndexes();
}
 
template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
ISAM<ObjType, MembType, T, IndexAmount>::~ISAM() {
    fstream file;
    file.open(this->name_file, ios::in | ios::out | ios::binary);
    fstream aux_file;
    aux_file.open(this->name_aux, ios::in | ios::out | ios::binary);

    vector<ObjType> all;
    ObjType record;
    file >> record;
    all.push_back(record);

    while(record.next.position != (long int)-1) {
        if(record.next.file == 'd') {
            file.seekg(record.next.position * (sizeof(ObjType) + 1));
            file >> record;
        } else {
            aux_file.seekg(record.next.position * (sizeof(ObjType) + 1));
            aux_file >> record;
        }
        
        all.push_back(record);
    }

    file.close();
    aux_file.close();

    file.open(this->name_file, ios::in | ios::out | ios::binary);

    for(auto r : all) {
        file << r;
    }
}

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

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

    file.seekg(0, ios::beg);
    file.close();

    this->size_file = records.size();

    sort(records.begin(), records.end(), [this](const ObjType& r1, const ObjType& r2) {
        return atoi(r1.*this->primary_key) < atoi(r2.*this->primary_key);
    });

    file.open(this->name_file, ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << "Can't open " << this->name_file << endl;
        return;
    }

    for(long int i = 1; i <= (long int)records.size(); ++i) {
        if(i != (long int)records.size()) {
            records[i - 1].next.position = i;
        } else {
            records[i - 1].next.position = -1;
        }
        
        file << records[i - 1];
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

    this->entries_per_pages = round(pow(leaf_pages, (double)1/(double)IndexAmount));

    for(size_t i = 0; i < leaf_pages * leaf_entries; ++i) {
        if(i % (leaf_entries - 1) == 0) {
            indexer_leaf<ObjType, MembType>(indexes[IndexAmount - 1], file, sizeof(ObjType) + 1, i, this->primary_key);
        }
    }

    file.close();

    leaf_entries = leaf_pages;
    leaf_pages /= entries_per_pages;

    for(int i = (int)IndexAmount - 2; i >= 0; --i) {
        leaf_entries = leaf_pages;
        leaf_pages /= entries_per_pages;

        for(size_t j = 0; j < leaf_pages * leaf_entries; ++j) {
            if(j % (leaf_entries - 1) == 0) {
                indexer(indexes[i], indexes[i + 1], sizeof(index) + 1, j);
            }
        }
    }

    for(size_t i = 0; i < IndexAmount; ++i) {
        indexes[i].close();
    }
}

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
bool ISAM<ObjType, MembType, T, IndexAmount>::search_in_tree(fstream& file, fstream& aux_file, const T& key, long int& position, pair<ObjType, char>& result) {
    fstream indexes[IndexAmount];

    for(size_t i = 0; i < IndexAmount; ++i) {
        indexes[i].open(this->index_name[i], ios::in | ios::out | ios::binary);

        if(!indexes[i].is_open()) {
            cerr << RED << "Can't open " << this->index_name[i] << RESET << endl;
            return false;
        }
    }

    for(size_t i = 0; i < IndexAmount; ++i) {
        if(i == 0) {
            while(indexes[i] >> this->index && atoi(index.primary_key) < stoi(key)) {
                position = index.pos;
            }
        } else if(i == IndexAmount - 1) {
            position = linear_search_index<T>(indexes[i], key, this->entries_per_pages * this->entries_per_pages, position);
        }
        else {
            position = linear_search_index<T>(indexes[i], key, this->entries_per_pages, position);
        }
    }

    bool is_in_file = linear_search_leaf<ObjType, MembType, T>(file, aux_file, key, primary_key, MAX_LEAF_ENTRIES * this->entries_per_pages, position, result);

    file.close();
    aux_file.close();

    for(size_t i = 0; i < IndexAmount; ++i) {
        indexes[i].close();
    }

    return is_in_file;
}

//(-_-)(-_-)(-_-)(-_-)(-_-)(-_-)(-_-)(-_-)(-_-)(-_-)(-_-)(-_-)(-_-)(-_-)(-_-)(-_-)(-_-)

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
ObjType ISAM<ObjType, MembType, T, IndexAmount>::search(const T& key) {
    fstream file;
    file.open(this->name_file, ios::in | ios::out | ios::binary);
    fstream aux_file;
    aux_file.open(this->name_aux, ios::in | ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << RED << "Can't open " << this->name_file << RESET << endl;
        throw new exception;
    }

    if(!aux_file.is_open()) {
        cerr << RED << "Can't open " << this->name_aux << RESET << endl;
        throw new exception;
    }

    long int position = 0;
    pair<ObjType, char> result;
    bool is_in_file = search_in_tree(file, aux_file, key, position, result);

    file.close();
    aux_file.close();

    if(is_in_file) {
        return result.first;
    }

    cerr << RED << "Record " << YELLOW << key << RED << " doesn't exists" << RESET << endl;
    throw new exception;
}

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
vector<ObjType> ISAM<ObjType, MembType, T, IndexAmount>::search(const T& begin, const T& end) {
    fstream file;
    file.open(this->name_file, ios::in | ios::out | ios::binary);
    fstream aux_file;
    aux_file.open(this->name_aux, ios::in | ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << RED << "Can't open " << this->name_file << RESET << endl;
        throw new exception;
    }

    if(!aux_file.is_open()) {
        cerr << RED << "Can't open " << this->name_aux << RESET << endl;
        throw new exception;
    }

    long int position = 0;
    pair<ObjType, char> result;
    bool is_in_file = search_in_tree(file, aux_file, begin, position, result);

    if(!is_in_file) {
        cerr << RED << "Record doesn't exists" << RESET << endl;
        throw new exception;
    }

    file.close();
    aux_file.close();

    file.open(this->name_file, ios::in | ios::out | ios::binary);
    aux_file.open(this->name_aux, ios::in | ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << RED << "Can't open " << this->name_file << RESET << endl;
        throw new exception;
    }

    if(!aux_file.is_open()) {
        cerr << RED << "Can't open " << this->name_aux << RESET << endl;
        throw new exception;
    }


    ObjType temp = result.first;
    char f = result.second;
    position = temp.next.position;


    vector<ObjType> all;
    all.push_back(temp);

    while(atoi(temp.*primary_key) < stoi(end) && temp.next.position != -1) {
        if(f == 'd') {
            file.seekg(temp.next.position * (sizeof(ObjType) + 1));
            file >> temp;
            f = temp.next.file;
            position = temp.next.position;
            all.push_back(temp);
        } else {
            aux_file.seekg(temp.next.position * (sizeof(ObjType) + 1));
            aux_file >> temp;
            f = temp.next.file;
            position = temp.next.position;
            all.push_back(temp);
        }
    }

    file.close();
    aux_file.close();

    return all;
}

template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
void ISAM<ObjType, MembType, T, IndexAmount>::add(ObjType record) {
    fstream file;
    file.open(this->name_file, ios::in | ios::out | ios::binary);
    fstream aux_file;
    aux_file.open(this->name_aux, ios::in | ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << RED << "Can't open " << this->name_file << RESET << endl;
        throw new exception;
    }

    if(!aux_file.is_open()) {
        cerr << RED << "Can't open " << this->name_aux << RESET << endl;
        throw new exception;
    }

    long int position = 0;
    pair<ObjType, char> result;
    bool is_in_file = search_in_tree(file, aux_file, record.*primary_key, position, result);

    file.close();
    aux_file.close();

    if(is_in_file) {
        cerr << RED << "Record exists\n" << RESET;
        return ;
    }

    file.open(this->name_file, ios::in | ios::out | ios::binary);
    aux_file.open(this->name_aux, ios::in | ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << RED << "Can't open " << this->name_file << RESET << endl;
        throw new exception;
    }

    if(!aux_file.is_open()) {
        cerr << RED << "Can't open " << this->name_aux << RESET << endl;
        throw new exception;
    }

    char f = result.second;
    ObjType temp;

    if(f == 'd') {
        file.seekg(position * (sizeof(ObjType) + 1));
        file >> temp;
        aux_file.seekg(0, ios::end);
        long int temp_position = aux_file.tellg() / (sizeof(ObjType) + 1);

        record.next = temp.next;
        temp.next.position = temp_position;
        temp.next.file = 'a';

        swap(record, temp);
        swap(record.next, temp.next);

        aux_file << record;
        file.seekg(position * (sizeof(ObjType) + 1));
        file << temp;
    } else {
        aux_file.seekg(position * (sizeof(ObjType) + 1));
        aux_file >> temp;
        aux_file.seekg(0, ios::end);
        long int temp_position = aux_file.tellg() / (sizeof(ObjType) + 1);

        record.next = temp.next;
        temp.next.position = temp_position;
        temp.next.file = 'a';

        swap(record, temp);
        swap(record.next, temp.next);

        aux_file << record;
        aux_file.seekg(position * (sizeof(ObjType) + 1));
        aux_file << temp;
    }
}

#endif //ISAM_H

