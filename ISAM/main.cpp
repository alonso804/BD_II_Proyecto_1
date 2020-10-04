#include"./record/record.h"
#include"./isam/isam.h"

int main(int argc, char const *argv[]) {
    ISAM<Record, char(Record::*)[20], string, 3> isam("data", &Record::name);
     
    return 0;
}

