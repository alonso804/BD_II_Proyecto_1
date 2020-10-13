#include"./sequential/sequential.h"
#include"./csv/reader.h"


int main(int argc, char const *argv[]) {
    ofstream file("data.dat", ios::binary);
    if(!file.is_open()) {
        cerr << "Error" << endl;
    }
    auto data = read_record(); 
    vector<Aeropuerto> all;

    for(auto rows : data) {
        Aeropuerto aeropuerto (rows[0], rows[1],rows[2],rows[3],rows[4],rows[5], rows[6], rows[7], rows[8],rows[9],rows[10],rows[11], rows[12]);

        all.push_back(aeropuerto);
    }

    file.close();

    SequentialFile<Aeropuerto, char(Aeropuerto::*)[6]> sequential("data", &Aeropuerto::id);
    sequential.insertAll(all);
    Aeropuerto result;

    cout << "TEST 1\n";
    for(auto i : data) {
        result = sequential.search(i[0]);
    }

    cout << "TEST 2\n";
    auto result_1 = sequential.search(data[0][0], data[6450][0]);

    for(auto i : result_1) {
        i.print();
    }

    Aeropuerto temp_1 ("118","Peace River Airport","Moncton","Canada","YQM","CYQM","46.11220169067383","-64.67859649658203","232","-4","A","America/Halifax","airport");
    sequential.add(temp_1);

    result = sequential.search("118");
    result.print();
    result = sequential.search("119");
    result.print();

    Aeropuerto temp_2 ("223","Touat Cheikh Sidi Mohamed Belkebir Airport","Adrar","Algeria","AZR","DAUA","27.837600708007812","-0.18641400337219238","919","1","N","Africa/Algiers","airport");
    sequential.add(temp_2);

    result = sequential.search("223");
    result.print();
    result = sequential.search("224");
    result.print();

    Aeropuerto temp_3 ("321","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_3);

    result = sequential.search("321");
    result.print();
    result = sequential.search("322");
    result.print();

    return 0;
}
