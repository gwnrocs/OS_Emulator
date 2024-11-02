#include "mainConsole.h"
#include "consoleManager.h"
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

void MainConsole::onEnabled()
{
    this->process();
}

void MainConsole::display()
{
    Utils::printHeader();
}

//TODO: Show header only once
void MainConsole::process()
{
    while (true) {
        display();
        std::string command;
        std::cout << " ";
        std::getline(std::cin >> std::ws, command);
        processCommand(command);
    }
}

void MainConsole::processCommand(std::string command) {
        /*std::string command;
        std::getline(std::cin, command);*/

        // Check if the system has been initialized
        if (!initialized && command != "initialize") {
            Utils::printError("You must first run the 'initialize' command.");
            return;  // Skip processing the command if the system isn't initialized
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
            ProcessManager::createProcess(command.substr(10));
        }
        else if (command.substr(0, 9) == "screen -r") {
            //TODO: Check if process is done
            if (ProcessManager::checkProcessExist(command.substr(10)) == 0) 
            {
                std::cout << "Process " << command.substr(10) << " not found.\n";
            }
            else 
            {
                ConsoleManager::getInstance()->switchConsole(command.substr(10));
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