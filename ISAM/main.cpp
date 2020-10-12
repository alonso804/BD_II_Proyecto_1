#include"./record/record.h"
#include"./isam/isam.h"
#include"./csv/reader.h"

int main(int argc, char const *argv[]) {
    ofstream file("data.dat", ios::binary);
    if(!file.is_open()) {
        cerr << "Error" << endl;
    }
    auto data = read_record(); 
    for(auto rows : data) {
        Aeropuerto aeropuerto (rows[0], rows[1],rows[2],rows[3],rows[4],rows[5], rows[6], rows[7], rows[8],rows[9],rows[10],rows[11], rows[12]);
        file << aeropuerto;
    }

    ISAM<Aeropuerto, char(Aeropuerto::*)[6], string, 3> isam("data", &Aeropuerto::id);
     
    return 0;
}

