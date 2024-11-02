#include "utils.h"

void loadConfig(int& num_cpu, string& scheduler, int& quantum_cycles, int& min_ins, int& max_ins, int& batch_freq, int& delays_per_exec) {
    ifstream configFile("config.txt");
    if (!configFile) {
        cout << "Error opening config.txt file!" << endl;
        return;
    }

    string param;
    while (configFile >> param) {
        if (param == "num-cpu") configFile >> num_cpu;
        else if (param == "scheduler") configFile >> scheduler;
        else if (param == "quantum-cycles") configFile >> quantum_cycles;
        else if (param == "min-ins") configFile >> min_ins;
        else if (param == "max-ins") configFile >> max_ins;
        else if (param == "batch-process-freq") configFile >> batch_freq;
        else if (param == "delays-per-exec") configFile >> delays_per_exec;
    }
    configFile.close();
}

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
    cout << "\t   ______ _____   ____   ____   ______ _______  __       \n"
        << "\t  / ____// ___/  / __ \\ / __ \\ / ____// ___/\\ \\/ /   \n"
        << "\t / /     \\__  \\ / / / // /_/ // __/   \\__ \\  \\  /   \n"
        << "\t/ /___  ___ / // /_/ // ____// /___  ___/ /  / /         \n"
        << "\t\\____/ /____ / \\____//_/    /_____/ /____/  /_/      \n\n";
}

void Utils::printConfirmation(const string& command) {
    cout << Colors::Green << "\n  " << command << " command recognized. Doing something. . ." << Colors::White;
    Sleep(2000);
    system("cls");
}

void Utils::clearScreen() {
    printConfirmation("clear");
    system("cls");
}

void Utils::printError(const string& command) {
    cout << Colors::Red << "\n  " << command << " command not recognized. Clearing menu . . ." << Colors::White;
    Sleep(2000);
    system("cls");
}

void Utils::printHeader() {
    printAscii();
    cout << Colors::Green << "  Hello, Welcome to CSOPESY commandline!\n";
    cout << Colors::Yellow << "  Type 'exit' to quit, 'clear' to clear the screen\n\n" << Colors::White;
    cout << "  Enter a command: ";
}
