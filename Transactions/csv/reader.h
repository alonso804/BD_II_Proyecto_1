#ifndef Reader_H
#define Reader_H

#include"../header.h"

vector<vector<string>> read_record() { 
	fstream fin;
    fin.open("./data/Aeropuerto.csv", ios::in); 

    if(fin.is_open()) {
        vector<vector<string>> data;
        vector<string> row;
        string line, word, temp; 

        while (getline(fin, line, '\n')) { 
            row.clear(); 
            stringstream s(line); 

            while (getline(s, word, ',')) { 
                row.push_back(word); 
            } 

            data.push_back(row);
        }

        return data;
    } else {
        cerr << "No abre\n";
    }

    throw new exception;
} 


#endif //Reader_H
