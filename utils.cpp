#include "utils.h"

//void loadConfig(int& num_cpu, string& scheduler, int& quantum_cycles, int& min_ins, int& max_ins, 
//                int& batch_freq, int& delays_per_exec) {
//    ifstream configFile("config.txt");
//    if (!configFile) {
//        cout << "Error opening config.txt file!" << endl;
//        return;
//    }
//
//    string param;
//    while (configFile >> param) {
//        if (param == "num-cpu") configFile >> num_cpu; 
//        else if (param == "scheduler") configFile >> scheduler;
//        else if (param == "quantum-cycles") configFile >> quantum_cycles;
//        else if (param == "min-ins") configFile >> min_ins;
//        else if (param == "max-ins") configFile >> max_ins;
//        else if (param == "batch-process-freq") configFile >> batch_freq;
//        else if (param == "delays-per-exec") configFile >> delays_per_exec;
//    }
//    configFile.close();
//}

bool loadConfig(int& num_cpu, string& scheduler, int& quantum_cycles, int& min_ins, int& max_ins,
                int& batch_freq, int& delays_per_exec) {
    ifstream configFile("config.txt");
    if (!configFile) {
        cout << "\nError opening config.txt file!" << endl;
        return false;
    }

    unsigned int temp_num_cpu;
    unsigned int temp_quantum_cycles, temp_min_ins, temp_max_ins, temp_batch_freq, temp_delays_per_exec;
    string temp_scheduler;
    const unsigned long long maxValue = 4294967296; // 2^32
    bool valid = true;
    string param;

    while (configFile >> param) {
        if (param == "num-cpu") {
            configFile >> temp_num_cpu;
            if (temp_num_cpu <= 0 || temp_num_cpu > 128) {
                cout << "\n  Invalid value for num-cpu: " << temp_num_cpu << 
                        ". Must be within range [1, 128]." << endl;
                valid = false; break;
            }
        }
        else if (param == "scheduler") {
            configFile >> temp_scheduler;
            if (temp_scheduler != "fcfs" && temp_scheduler != "rr") {
                cout << "\n  Invalid value for scheduler: " << temp_scheduler << 
                        ". Must be 'fcfs' or 'rr'." << endl;
                valid = false; break;
            }
        }
        else if (param == "quantum-cycles") {
            configFile >> temp_quantum_cycles;
            if (temp_quantum_cycles < 1 || temp_quantum_cycles > maxValue) {
                cout << "\n  Invalid value for quantum-cycles: " << temp_quantum_cycles << 
                        ". Must be in range [1, 2^32]." << endl;
                valid = false; break;
            }
        }
        else if (param == "min-ins") {
            configFile >> temp_min_ins;
            if (temp_min_ins < 1 || temp_min_ins > maxValue) {
                cout << "\n  Invalid value for min-ins: " << temp_min_ins << 
                        ". Must be in range [1, 2^32]." << endl;
                valid = false; break;
            } 
        }
        else if (param == "max-ins") {
            configFile >> temp_max_ins;
            if (temp_max_ins < 1 || temp_max_ins > maxValue || temp_max_ins < temp_min_ins) {
                cout << "\n  Invalid value for max-ins: " << temp_max_ins << 
                        ". Must be in range [1, 2^32] and more than min-ins." << endl;
                valid = false; break;
            }
        }
        else if (param == "batch-process-freq") {
            configFile >> temp_batch_freq;
            if (temp_batch_freq < 1 || temp_batch_freq > maxValue) {
                cout << "\n  Invalid value for batch-process-freq: " << temp_batch_freq << 
                        ". Must be in range [1, 2^32]." << endl;
                valid = false; break;
            }
        }
        else if (param == "delays-per-exec") {
            configFile >> temp_delays_per_exec;
            if (temp_delays_per_exec < 0 || temp_delays_per_exec > maxValue) {
                cout << "\n  Invalid value for delays-per-exec: " << temp_delays_per_exec << 
                        ". Must be in range [0, 2^32]." << endl;
                valid = false; break;
            }
        }
    }
    configFile.close();

    // If all inputs are valid, update the original variables
    if (valid) {
        num_cpu = temp_num_cpu;
        scheduler = temp_scheduler;
        quantum_cycles = temp_quantum_cycles;
        min_ins = temp_min_ins;
        max_ins = temp_max_ins;
        batch_freq = temp_batch_freq;
        delays_per_exec = temp_delays_per_exec;
        return true;
    }
    else {
        cout << "  Configuration loading failed due to invalid inputs.\n  Clearing screen . . ." << endl;
        return false;
    }
}

//bool configFileExists() {
//    ifstream file("config.txt");
//    return file.is_open(); 
//}

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