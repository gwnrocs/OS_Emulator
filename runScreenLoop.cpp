#include "runScreenLoop.h"

void runScreenLoop(function<void()> action) {
    string command;
    bool smi = false;

    while (command != "exit") {
        if (smi == false) {
            system("cls");
        } else {
            cout << "\n";
        }
        
        action();
        cout << Colors::Yellow << "  Enter command: " << Colors::White;
        getline(cin, command);

        if (command == "exit") {
            break;
        }
        else if ("process-smi") {
            smi = true;
            continue;
        }
        else {
            Utils::printError(command);
        }
    }
    system("cls");
}