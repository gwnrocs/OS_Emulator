#include "mainConsole.h"
#include <iostream>

MainConsole::MainConsole() : running(true), initialized(false), scheduler() {
    int num_cpu, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec;
    std::string scheduler_type;

    // Load the number of CPU cores from config.txt
    loadConfig(num_cpu, scheduler_type, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec);

    // Dynamically allocate the CPU object based on num_cpu
    cpu = new CPU(num_cpu); 
}

MainConsole::~MainConsole() {
    if (cpu != nullptr) {
        delete cpu;  // Free the dynamically allocated CPU object
    }
}


void MainConsole::printHeading() {
    Utils::printHeader(); 
}

void MainConsole::drawConsole() {
    while (running) {
        Utils::printHeader();
        std::string command;
        std::getline(std::cin, command);

        // Check if the system has been initialized
        if (!initialized && command != "initialize") {
            Utils::printError("You must first run the 'initialize' command.");
            continue;  // Skip processing the command if the system isn't initialized
        }

        if (command == "initialize") {
            int num_cpu, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec;
            std::string scheduler_type;

            // Load the config.txt file and initialize the CPU and other parameters
            loadConfig(num_cpu, scheduler_type, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec);

            cpu = new CPU(num_cpu);  // Initialize CPU with the loaded number of cores
            initialized = true;  // Set the initialized flag to true
            Utils::printConfirmation("System initialized.");
        }
        else if (command == "exit") {
            running = false;
        }
        else if (command == "clear") {
            Utils::clearScreen();
        }
        else if (command.substr(0, 9) == "screen -s") {
            std::string processName = command.substr(10);

            if (processName.empty() || std::all_of(processName.begin(), processName.end(), ::isspace)) {
                std::cout << "  Error: No process name provided for 'screen -s'.\n";
            } else {
            ProcessManager::createProcess(command.substr(10));
            }
        }
        else if (command.substr(0, 9) == "screen -r") {
            //TODO: Check if process is done
            std::string processName = command.substr(10);

            if (processName.empty() || std::all_of(processName.begin(), processName.end(), ::isspace)) {
                std::cout << "  Error: No process name provided for 'screen -s'.\n";
            }
            else {
                if (ProcessManager::checkProcessExist(command.substr(10)) == 0)
                {
                    std::cout << "  Process " << command.substr(10) << " not found.\n";
                }
                else {
                    ProcessManager::redrawProcess(command.substr(10));
                }
            }
        }
        else if (command == "scheduler -test") {
            Commands::schedulerTest(*cpu, scheduler);
        }
        else if (command == "screen -ls") {
            Commands::screenList();
        }
        else if (command == "scheduler -stop") {
            Commands::schedulerStop();
        }
        else if (command == "report-util") {
            Commands::reportUtil();
        }
        else {
            Utils::printError(command);
        }
    }
}

