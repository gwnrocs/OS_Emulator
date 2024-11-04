#include<iostream>
#include<string>
#include "MainConsole.h"

int main() {
    MainConsole* mainConsole = new MainConsole();

    mainConsole->drawConsole();   

    delete mainConsole;  
    return 0;
}