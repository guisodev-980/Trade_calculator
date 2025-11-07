#include "menu.h"
#include "cruddata.h"
#include "printers.h"
#include <iostream>

int main(){
    readFromCsv();
    readFromJson();
    while(true){
        screenClear();
        menu();
    }
    return 0;
}