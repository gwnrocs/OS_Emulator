#include "runScreenLoop.h"

void runScreenLoop(function<void()> action) {
    string command;
    bool smi = true;
    system("cls");

    while (command != "exit") {
        if (smi == true) 
            action();

        cout << Colors::Yellow << "  Enter command: " << Colors::White;
        getline(cin, command); 

        if (command == "exit") {
            break;
        }
        else if (command == "process-smi") {
            smi = true; 
        }
        else if (command == "screen -ls") {
            smi = false;
        }
        else {
            smi = false;
            cout << Colors::Red << "\n Command not recognized.\n\n" << Colors::White; 
        }
    }
    system("cls"); 
    Utils::printHeader();
}