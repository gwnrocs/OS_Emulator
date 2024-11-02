#include "runScreenLoop.h"

void runScreenLoop(function<void()> action) {
    string command;
    while (command != "exit") {
        system("cls");
        action();
        cout << Colors::Yellow << "  Type 'exit' to return back to main menu: " << Colors::White;
        getline(cin, command);

        if (command == "exit") {
            break;
        }
        else {
            Utils::printError(command);
        }
    }
    system("cls");
}
