#include "mainConsole.h"
#include <iostream>
#include <thread>

MainConsole::MainConsole() : running(true), initialized(false), scheduler(std::make_shared<Scheduler>()) {
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
    Utils::printHeader();

    while (running) {
        cout << "\n  Enter a command: ";
        std::string command;
        std::getline(std::cin, command);

        // Check if the system has been initialized
        if (!initialized && command != "initialize") {
            Utils::printError("You must first run the 'initialize' command.");
            continue; 
        }

        if (command == "initialize") {
            num_cpu, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec;
            scheduler_type;

            // Load the config.txt file
            bool configSuccess = loadConfig(num_cpu, scheduler_type, quantum_cycles, min_ins, 
                                            max_ins, batch_freq, delays_per_exec);

            if (configSuccess) {
                cpu = new CPU(num_cpu); 
                initialized = true;  

                std::thread schedulerThread([this] { scheduler->runScheduler(*cpu); });
                schedulerThread.detach();

                Utils::printConfirmation("System initialized.");
            }
            else {
                Sleep(2500); system("cls");
            }
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
            ProcessManager::createProcess(scheduler, command.substr(10));
            ProcessManager::redrawProcess(command.substr(10));
            }
        }
        else if (command.substr(0, 9) == "screen -r") {
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
            Commands::schedulerTest(scheduler, batch_freq);
        }
        else if (command == "screen -ls") {
            Commands::screenList(*cpu);
        }
        else if (command == "scheduler -stop") {
            Commands::schedulerStop();
        }
        else if (command == "report-util") {
            Commands::reportUtil(*cpu);
        }
        else {
            Utils::printError(command);
        }
    }
}