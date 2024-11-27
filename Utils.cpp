#include "utils.h"

int Utils::generateRandomNumber(int lower, int upper) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(lower, upper);
    return dist(gen);
}

string Utils::getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    auto now_c = chrono::system_clock::to_time_t(now);
    tm local_tm;
    localtime_s(&local_tm, &now_c);
    ostringstream oss;
    oss << put_time(&local_tm, "%m/%d/%Y %I:%M:%S%p");
    return oss.str();
}

void Utils::printAscii() {
    cout << "      ______ _____   ____   ____   ______ _______  __       \n"
        << "     / ____// ___/  / __ \\ / __ \\ / ____// ___/\\ \\/ /   \n"
        << "    / /     \\__  \\ / / / // /_/ // __/   \\__ \\  \\  /   \n"
        << "   / /___  ___ / // /_/ // ____// /___  ___/ /  / /         \n"
        << "   \\____/ /____ / \\____//_/    /_____/ /____/  /_/      \n\n";
}

void Utils::printConfirmation(const string& command) {
    cout << Colors::Green << "\n" << command << " command recognized. Doing something. . ." << Colors::White;
    Sleep(1000);
    system("cls");
    Utils::printHeader();
}

void Utils::clearScreen() {
    printConfirmation("clear");
    system("cls");
    Utils::printHeader();
}

void Utils::printError(const string& command) {
    cout << Colors::Red << "\n" << command << " command not recognized. Clearing menu . . ." << Colors::White;
    Sleep(1000);
    system("cls");
    Utils::printHeader();
}

void Utils::printHeader() {
    printAscii();
    cout << Colors::Green << "Hello, Welcome to CSOPESY commandline!\n";
    cout << Colors::Yellow << "Type 'exit' to quit, 'clear' to clear the screen\n\n" << Colors::White;
}

void Utils::printDivider() {
    cout << Colors::Grey << "\n---------------------------------- \n" << Colors::White << endl;
}