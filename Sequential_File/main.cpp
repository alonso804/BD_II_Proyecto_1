#include"./sequential/sequential.h"
#include"./csv/reader.h"
       
double get_Time()
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec + now.tv_nsec*1e-9;
}

int main(int argc, char const *argv[]) {
    
    double actualtime;
    double timespent;

    actualtime = get_Time();
    timespent = get_Time() - actualtime;

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
    /*
    cout << "TEST 1\n";
    for(auto i : data) {
        result = sequential.search(i[0]);
    }

    cout << "TEST 2\n";
    auto result_1 = sequential.search(data[0][0], data[6450][0]);

    for(auto i : result_1) {
        i.print();
    }
    
    cout<<endl<<endl;
    cout<<"Test............................................."<<endl<<endl;
    
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
    
    Aeropuerto temp_3 ("473","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_3);
    Aeropuerto temp_4 ("321","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_4);
    Aeropuerto temp_5 ("527","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_5);
    Aeropuerto temp_6 ("598","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_6);
    Aeropuerto temp_7 ("606","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_7);
    Aeropuerto temp_8 ("620","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_8);
    Aeropuerto temp_9 ("661","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_9);
    Aeropuerto temp_10 ("672","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_10);
    Aeropuerto temp_11 ("732","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_11);
    Aeropuerto temp_12 ("744","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_12);
    Aeropuerto temp_13 ("747","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_13);
    Aeropuerto temp_14 ("749","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_14);
    Aeropuerto temp_15 ("752","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_15);
    Aeropuerto temp_16 ("776","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_16);
    Aeropuerto temp_17 ("805","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_17);
    Aeropuerto temp_18 ("826","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_18);
    Aeropuerto temp_19 ("830","Flugplatz Bautzen","Bautzen","Germany","null","EDAB","51.193611","14.519722","568","1","E","Europe/Berlin","airport");
    sequential.add(temp_19);
    
    result = sequential.search("321");
    result.print();
    result = sequential.search("322");
    result.print();
    */
    int contador = 100;
    for(int i=0;i<contador;++i)
    {
        result = sequential.search("322");
        /*
        Aeropuerto temp_1 (to_string(i),"Peace River Airport","Moncton","Canada","YQM","CYQM","46.11220169067383","-64.67859649658203","232","-4","A","America/Halifax","airport");
        sequential.add(temp_1);
        */
    }
    
    cout<<endl;
    printf("Time %.7f\n", timespent);
    cout<<endl;

    return 0;
}
