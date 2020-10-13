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
    file.close();

    ISAM<Aeropuerto, char(Aeropuerto::*)[6], string, 3> isam("data", &Aeropuerto::id);
    Aeropuerto result;

    cout << "TEST 1\n";
    for(size_t i = 0; i < 6451; ++i) {
        result = isam.search(data[i][0]);
        result.print();
    }

    cout << "TEST 2\n";
    auto result_1 = isam.search(data[0][0], data[6450][0]);

    for(auto i : result_1) {
        i.print();
    }

    cout << "TEST 3\n";
    Aeropuerto temp_1 ("118","Peace River Airport","Moncton","Canada","YQM","CYQM","46.11220169067383","-64.67859649658203","232","-4","A","America/Halifax","airport");
    isam.add(temp_1);

    result = isam.search("118");
    result.print();
    result = isam.search("119");
    result.print();

    Aeropuerto temp_2 ("223","Touat Cheikh Sidi Mohamed Belkebir Airport","Adrar","Algeria","AZR","DAUA","27.837600708007812","-0.18641400337219238","919","1","N","Africa/Algiers","airport");
    isam.add(temp_2);

    result = isam.search("223");
    result.print();
    result = isam.search("224");
    result.print();

    Aeropuerto temp_3 ("321","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    isam.add(temp_3);

    result = isam.search("321");
    result.print();
    result = isam.search("322");
    result.print();

    return 0;
}

