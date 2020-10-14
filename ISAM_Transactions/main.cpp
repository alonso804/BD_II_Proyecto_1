#include"./isam/isam.h"
#include"./csv/reader.h"

mutex mu;

void insertion(ISAM<Aeropuerto, char(Aeropuerto::*)[6], string, 3> isam, Aeropuerto temp) {
    mu.lock();
    isam.add(temp);
    cout << "Added!!\n";
    mu.unlock();
}

void searching(ISAM<Aeropuerto, char(Aeropuerto::*)[6], string, 3> isam, string key) {
    mu.lock();
    Aeropuerto result;
    result = isam.search(key);
    result.print();
    cout << "Finded!!\n";
    mu.unlock();
}

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

    Aeropuerto temp_1 ("118","Peace River Airport","Moncton","Canada","YQM","CYQM","46.11220169067383","-64.67859649658203","232","-4","A","America/Halifax","airport");

    thread t1 (searching, isam, "119");
    thread t2 (insertion, isam, temp_1);
    thread t3 (searching, isam, "118");

    t1.join();
    t2.join();
    t3.join();

    return 0;
}

