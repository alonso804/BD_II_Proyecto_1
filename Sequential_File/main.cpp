#include"./sequential/sequential.h"

template <typename T>
void searchTest(T&);

template <typename T>
void addTest(T&);

template <typename T>
void addTest2(T&);

template <typename T>
void joinFileTest(T&);

int main(int argc, char const *argv[]) {
    Registro registro1{"0001", "Alonso", "CS", 2};
    Registro registro2{"0003", "Lucero", "Ing Civil", 3};
    Registro registro3{"0004", "Noemi", "Turismo", 1};
    Registro registro4{"0002", "Diana", "Bioingenieria", 8};
    Registro registro5{"0111", "Alejandro", "Arquitectura", 3};
    Registro registro6{"0222", "Violeta", "CS", 2};

    vector<Registro> records;
    records.push_back(registro1);
    records.push_back(registro2);
    records.push_back(registro3);
    records.push_back(registro4);
    records.push_back(registro5);
    records.push_back(registro6);

    SequentialFile<Registro, char (Registro::*)[20]> test1("test1", &Registro::nombre);

    test1.insertAll(records);

    /*
     *cout << "=========================================\n";
     *cout << "SEARCH TEST 1\n";
     *cout << "=========================================\n";
     *searchTest(test1);
     */

    cout << "=========================================\n";
    cout << "ADD TEST 1\n";
    cout << "=========================================\n";
    addTest(test1);

    cout << "=========================================\n";
    cout << "JOIN TEST 1\n";
    cout << "=========================================\n";
    joinFileTest(test1);

    auto records_join = test1.search("Alejandro", "Violeta");
    for(auto& r : records_join) {
        r.print();
        cout << "-------------------------------------------\n";
    }

    cout << "=========================================\n";
    cout << "ADD TEST 2\n";
    cout << "=========================================\n";
    addTest2(test1);

    test1.search("Rogelio").print();
    cout << "-------------------------------------------\n";
    test1.search("Sofia").print();

    /*
     *cout << "=========================================\n";
     *cout << "SEARCH TEST 2\n";
     *cout << "=========================================\n";
     *searchTest(test1);
     */


    cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
    auto records_join2 = test1.search("Alejandro", "Zupe");
    for(auto& r : records_join2) {
        r.print();
        cout << "-------------------------------------------\n";
    }

    return 0;
}

template <typename T>
void searchTest(T& test) {
    Registro record;
    record = test.search("Alonso");
    record.print();
    cout << "---------------------------------\n";
    record = test.search("Noemi");
    record.print();
    cout << "---------------------------------\n";
    record = test.search("Lucero");
    record.print();
    cout << "---------------------------------\n";
    record = test.search("Diana");
    record.print();
    cout << "---------------------------------\n";
    record = test.search("Alejandro");
    record.print();
    cout << "---------------------------------\n";
    record = test.search("Violeta");
    record.print();
    cout << "---------------------------------\n";

    //-Must be error
    /*
     *record = test.search("Beatriz");
     *record.print();
     */
}

template <typename T>
void addTest(T& test) {
    Registro searched;

    Registro registro1 {"0006", "Aurelio", "Ing Sistemas", 9};
    test.add(registro1);

    Registro registro2 {"0005", "Rogelio", "Derecho", 3};
    test.add(registro2);

    Registro registro3 {"0010", "Jacky", "Derecho", 8};
    test.add(registro3);

    Registro registro4 {"0011", "Zupe", "Profesor", 7};
    test.add(registro4);

    searched = test.search("Rogelio");
    searched.print();
    cout << "---------------------------------\n";

    searched = test.search("Jacky");
    searched.print();
    cout << "---------------------------------\n";

    searched = test.search("Aurelio");
    searched.print();
    cout << "---------------------------------\n";

    searched = test.search("Zupe");
    searched.print();
    cout << "---------------------------------\n";

    //-Must be error
    /*
     *searched = test.search("Chris");
     *searched.print();
     */
}

template <typename T>
void addTest2(T& test) {
    Registro registro1 {"1286", "Sofia", "Modelo", 21};
    test.add(registro1);
}

template <typename T>
void joinFileTest(T& test) {
    Registro registro1 {"0016", "Chris", "Deportista", 1};
    test.add(registro1);
}
