#include"./sequential/sequential.h"
#include"./csv/reader.h"

void insertion(SequentialFile<Aeropuerto, char(Aeropuerto::*)[6]> sequential, Aeropuerto temp) {
    sequential.add(temp);
    cout << "Added!!\n";
}

void searching(SequentialFile<Aeropuerto, char(Aeropuerto::*)[6]> sequential, string key) {
    Aeropuerto result;
    result = sequential.search(key);
    result.print();
    cout << "Finded!!\n";
}

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

    Aeropuerto temp_2 ("223","Touat Cheikh Sidi Mohamed Belkebir Airport","Adrar","Algeria","AZR","DAUA","27.837600708007812","-0.18641400337219238","919","1","N","Africa/Algiers","airport");

    thread t1 (searching, sequential, "224");
    thread t2 (insertion, sequential, temp_2);
    thread t3 (searching, sequential, "223");

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
