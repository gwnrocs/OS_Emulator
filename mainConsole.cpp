#include "mainConsole.h"
#include <iostream>
#include <thread>
#include <string>
#include "ConfigManager.h"
using Config::configParams;



using namespace std;

MainConsole::MainConsole() : running(true), initialized(false), scheduler(make_shared<Scheduler>()) {

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
        string command;
        getline(cin, command);

        // Check if the system has been initialized
        if (!initialized && command != "initialize") {
            Utils::printError("You must first run the 'initialize' command.");
            continue; 
        }

        if (command == "initialize") {
            bool isConfigLoadSuccess = Config::loadConfig("config.txt"); 
            
            if (isConfigLoadSuccess) {
                cpu = new CPU(); 
                initialized = true;  

                thread schedulerThread([this] { scheduler->runScheduler(*cpu); });
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
            string processName = command.substr(10);

            if (processName.empty() || all_of(processName.begin(), processName.end(), ::isspace)) {
                cout << "  Error: No process name provided for 'screen -s'.\n";
            } else {
            ProcessManager::createProcess(scheduler, command.substr(10));
            ProcessManager::redrawProcess(command.substr(10));
            }
        }
        else if (command.substr(0, 9) == "screen -r") {
            string processName = command.substr(10);

            if (processName.empty() || all_of(processName.begin(), processName.end(), ::isspace)) {
                cout << "  Error: No process name provided for 'screen -s'.\n";
            }
            else {
                if (ProcessManager::checkProcessExist(command.substr(10)) == 0)
                {
                    cout << "  Process " << command.substr(10) << " not found.\n";
                }
                else {
                    ProcessManager::redrawProcess(command.substr(10));
                }
            }
        }
        else if (command == "scheduler -test") {
            Commands::schedulerTest(scheduler);
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