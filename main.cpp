#include<iostream>
#include<string>
#include "mainConsole.h"
#include "consoleManager.h"

int main() {
    MainConsole* mainConsole = new MainConsole();

    //mainConsole->drawConsole();   

    ConsoleManager::initialize();
    // Start the loop
    ConsoleManager::getInstance()->switchConsole(MAIN_CONSOLE);
    ConsoleManager::destroy();

    delete mainConsole;  
    return 0;
}