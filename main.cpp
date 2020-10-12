#include<iostream>
#include<string>
#include<cstring>

using namespace std;

int main(int argc, char const *argv[]) {
    char b[] = "burro";
    char a[] = "ada";
    char c[] = "cerro";
    char d[] = "dia";
    char e[] = "epilepsia";
    char e1[] = "elsa";


    if(atoi(a) < atoi(b)) {
        cout << "true" << endl;
    } else {
        cout << "false" << endl;
    }

    if(strcmp(a, b) < 0) {
        cout << "true" << endl;
    } else {
        cout << "false" << endl;
    }

    return 0;
}

