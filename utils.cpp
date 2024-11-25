#include "utils.h"
#include "ConfigManager.h"

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
    cout << Colors::Red << "\n  " << command << " command not recognized. Clearing menu . . ." << Colors::White;
    Sleep(1000);
    system("cls");
    Utils::printHeader();
}

void Utils::printHeader() {
    printAscii();
    cout << Colors::Green << "  Hello, Welcome to CSOPESY commandline!\n";
    cout << Colors::Yellow << "  Type 'exit' to quit, 'clear' to clear the screen\n\n" << Colors::White;
}

void Utils::printConfig() {
    cout << Colors::Grey << "  Current Configuration:" << Colors::White << endl;
    cout << "  Number of CPUs: " << Config::configParams.num_cpu << endl;

    if (Config::configParams.scheduler_type == "rr") {
        cout << "  Scheduler Type: Round Robin" << endl;
        cout << "  Quantum Cycles: " << Config::configParams.quantum_cycles << endl;
    } else {
        cout << "  Scheduler Type: First Come First Serve" << endl;
    }

    cout << "  Batch Process Frequency: " << Config::configParams.batch_freq << endl;
    cout << "  Minimum Instructions: " << Config::configParams.min_ins << endl;
    cout << "  Maximum Instructions: " << Config::configParams.max_ins << endl;
    cout << "  Delays Per Execution: " << Config::configParams.delays_per_exec << endl;
    cout << "  Maximum Overall Memory: " << Config::configParams.max_overall_mem << " KB" << endl;
    cout << "  Memory Per Frame: " << Config::configParams.mem_per_frame << " KB" << endl;
    cout << "  Minimum Memory Per Process: " << Config::configParams.min_mem_per_proc << " KB" << endl;
    cout << "  Maximum Memory Per Process: " << Config::configParams.max_mem_per_proc << " KB" << endl << endl;
    cout << Colors::Grey << "  ---------------------------------- " << Colors::White<< endl;
}