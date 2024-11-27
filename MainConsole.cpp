#include "MainConsole.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>

using namespace std;

MainConsole::MainConsole() : isMainMenu(true) {}

void MainConsole::start() {
    Utils::printHeader();
    string command, option, processName;

    while (true) {
        if (showConfig) {
            printConfig();
            showConfig = false;
        }

        cout << Colors::Yellow << "Enter a command: " << Colors::White;
        std::getline(std::cin, command);

        
        // Check if the system has been initialized
        if (!initialized && command != "initialize") {
            Utils::printError("You must first run the 'initialize' command.");
            continue;
        }

        if (command == "initialize") {
            if (initialized) {
                // Stop all threads and clean up
                hasQuit = true;
                joinAllThreads();

                // Reset all states
                screens.clear();
                scheduler.readyQueue.clear();
                scheduler.coresAvailable.clear();
                listOfCoreThreads.clear();
                hasQuit = false;

                cout << "  System stopped. Reinitializing..." << endl;
            }

            initialize(); // Call the initialize function to reinitialize the system
            if (initialized) {
                cout << "System initialized successfully." << endl << endl;
            }
        }

        else if (command == "exit") {
            hasQuit = true;
            joinAllThreads();

            cout << "Exiting program... Goodbye!" << endl;
            break;
        }

        else if (command.find("screen") == 0) {

            std::istringstream iss(command.substr(6));
            iss >> option >> processName;

            handleScreenCommand(option, processName);
        }
        else if (command == "clear") {
            Utils::clearScreen(); 
            showConfig = true;
        }
        else if (command == "report-util")
        {
            ofstream fileOPESY;
            fileOPESY.open("csopesy-log.txt");


            // copied and pasted from screen -ls command
            int coresUsed = checkCoresUsed();
            fileOPESY << "--------------------------------------" << endl 
                      << "CPU Utilization: " << std::round(((coresUsed * 1.0) / scheduler.coresAvailable.size()) * 100) << "%" << endl
                      << "Cores used: " << coresUsed << endl
                      << "Cores available: " << scheduler.coresAvailable.size() - coresUsed << endl << endl
                      << "--------------------------------------" << endl;

            if (screens.empty()) {
                fileOPESY << "No screens attached." << endl;
                Utils::printDivider();
            }
            else {

                fileOPESY << "Running processes: " << endl;

                for (const auto& screen : screens) {
                    if (screen->status == Screen::RUNNING)
                    {
                        fileOPESY << screen->processName << "    ";
                        fileOPESY << "(" + screen->creationTime + ")    ";
                        fileOPESY << "Core: " + std::to_string(screen->core_id_assigned) << "    ";
                        fileOPESY << screen->currentLine << " / " << screen->totalLines << "\n";
                    }
                }

                fileOPESY << endl;
                fileOPESY << "Finished processes: " << endl;

                for (const auto& screen : screens) {
                    if (screen->status == Screen::FINISHED)
                    {
                        fileOPESY << screen->processName << "    ";
                        fileOPESY << "(" + screen->creationTime + ")  ";
                        fileOPESY << "Finished     ";
                        fileOPESY << screen->currentLine << " / " << screen->totalLines << "\n";
                    }
                }
                fileOPESY << "--------------------------------------" << endl;

            }
            fileOPESY.close();

            cout << "\nSuccessfully printed report-util.\n" << endl;
        }
        else if (command == "scheduler -test") {
            if (toStartCreatingProcess)
                cout << Colors::Green << "\nProcess generation is already activated . . .\n\n" << Colors::White;
            
            else 
                toStartCreatingProcess = true;
                cout << Colors::Green << "\nProcess generation has started . . .\n\n" << Colors::White;
        }

        else if (command == "scheduler -stop") {
            if (!toStartCreatingProcess)
                cout << Colors::Red << "\nProcess generation is not activated . . .\n\n" << Colors::White;
            else {
                toStartCreatingProcess = false;
                cout << Colors::Red << "\nProcess generation has stopped . . .\n\n" << Colors::White;
            }
        }

        else if (command == "process-smi") {
            int coresUsed = checkCoresUsed();
            int memoryUsage = memory->getMemoryUsage();
            cout << endl;
            cout << "-------------------------------------------------------------" << endl;
            cout << "|                       PROCESS-SMI                         |" << endl;
            cout << "-------------------------------------------------------------" << endl;
            cout << "CPU-Util: " << std::round(((coresUsed * 1.0) / scheduler.coresAvailable.size()) * 100) << "%" << endl;
            cout << "Memory Usage: " << memoryUsage << "KB / " << memory->maxMemory << "KB" << endl;
            cout << "Memory Util: " << std::round(((memoryUsage * 1.0) / memory->maxMemory) * 100) << "%" << endl << endl;
            cout << "==============================================================" << endl;
            cout << "Running processes and memory usage:" << endl;
            cout << "-------------------------------------------------------------" << endl;

            for (int i = 0; i < memory->processInMemory.size(); i++)
                cout << memory->processInMemory[i]->processName << " " << memory->processInMemory[i]->memory_to_occupy << "KB" << endl;

            cout << "-------------------------------------------------------------" << endl;
        }

        else if (command == "vmstat") {
            int memoryUsage = memory->getMemoryUsage();
            int freeMemory = memory->maxMemory - memoryUsage;
            Utils::printDivider();
            cout << memory->maxMemory << " total memory" << endl
                 << memoryUsage << " used memory" << endl
                 << freeMemory << " free memory" << endl
                 << idleCycles << " idle CPU ticks" << endl
                 << activeCycles << " active CPU ticks" << endl
                 << cpuCycles << " total CPU ticks" << endl
                 << memory->numPagedIn << " num paged in" << endl
                 << memory->numPagedOut << " num paged out" << endl;
            Utils::printDivider();
        }
        else {
            showConfig = true;
            Utils::printError(command);
        }
    }
}


void MainConsole::initializeCores(int numCores, int delay, int quantumCycles, std::shared_ptr<Memory> memory)
{
    for (int i = 0; i < numCores; i++) {
        auto core = make_shared<Core>(i, delay, quantumCycles, memory);
        scheduler.coresAvailable.push_back(core);
    }

    listOfCoreThreads.resize(numCores); // reserve space in advance
}


void MainConsole::handleScreenCommand(const string& option, const string& processName) {
    if (option == "-r" && processName != "") {
        attachScreen(processName);
    }
    else if (option == "-s" && processName != "") {
        createScreen(processName);
    }
    else if (option == "-ls") {
        listScreens();
    }
    else if (option == "-ls-debug") {
        listScreens(true);
    }
    else {
        cout << "Command not recognized." << endl;
    }
}


void MainConsole::printConfig() {
    cout << Colors::Grey << "Current Configuration:" << Colors::White << endl;

    cout << "Number of CPUs: " << nCpuToInitialize << endl;

    if (schedulerType == "rr") {
        cout << "Scheduler Type: Round Robin"            << endl
                  << "Quantum Cycles: " << quantumCycles << endl;
    }
    else {
        cout << "Scheduler Type: First Come First Serve" << endl
             << "Quantum Cycles: N/A"                    << endl;
    }

    cout << "Batch Process Frequency: "     << freqProcess              << endl
         << "Minimum Instructions: "        << minCommand               << endl
         << "Maximum Instructions: "        << maxCommand               << endl
         << "Delays Per Execution: "        << delayExecFake            << endl
         << "Maximum Overall Memory: "      << maxMemory      << " KB"  << endl
         << "Memory Per Frame: "            << memoryPerFrame << " KB"  << endl
         << "Minimum Memory Per Process: "  << minMemPerProc  << " KB"  << endl
         << "Maximum Memory Per Process: "  << maxMemPerProc  << " KB"  << endl;

    Utils::printDivider();
}


void MainConsole::createScreen(const string& processName) {
    if (!checkExistingScreen(processName)) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(minCommand, maxCommand); // randomize the amount of commands
        std::uniform_int_distribution<> mem(minMemPerProc, maxMemPerProc);
        int generatedMemory = mem(gen);

        int processId = screens.size();
        std::shared_ptr<Screen> screen = make_shared<Screen>(Screen(processName, processId, 0, dist(gen), 
                                        Utils::getCurrentTimestamp(), generatedMemory, 
                                        ceil((generatedMemory * 1.0) / memoryPerFrame)));
        screens.push_back(screen);
        scheduler.readyQueue.push_back(screen);
        initScreen(screen);
    }
    else {
        cout << "Screen initialization failed. Please use another process name." << endl;
    }
}

void MainConsole::attachScreen(const string& processName) {
    bool screenFound = false;
    for (int i = 0; i < screens.size(); i++) {
        if (screens[i]->processName == processName) {

            if (screens[i]->status == Screen::FINISHED)
                break;
            else
            {
                initScreen(screens[i]);
                screenFound = true;
                break;
            }
        }
    }
    if (!screenFound) {
        cout << "Process " << processName << " not found." << endl;
    }
}

void MainConsole::listScreens(bool debug) {

    int coresUsed = checkCoresUsed();

    Utils::printDivider();
    cout << "CPU Utilization: " << std::round(((coresUsed * 1.0) / scheduler.coresAvailable.size()) * 100) << "%" << endl
         << "Cores used: " << coresUsed << endl
         << "Cores available: " << scheduler.coresAvailable.size() - coresUsed << endl;
    Utils::printDivider();

    if (screens.empty()) {
        cout << "No screens attached." << endl;
    }
    else {
        cout << "Running processes: " << endl;

        for (const auto& screen : screens) {
            if (screen->status == Screen::RUNNING)
            {
                cout << screen->processName << "    "
                     << "(" + screen->creationTime + ")    "
                     << "Core: " + std::to_string(screen->core_id_assigned) << "    "
                     << screen->currentLine << " / " << screen->totalLines << endl;
            }
        }

        cout << endl;
        cout << "Finished processes: " << endl;

        for (const auto& screen : screens) {
            if (screen->status == Screen::FINISHED)
            {
                cout << screen->processName << "    "
                     << "(" + screen->creationTime + ")  "
                     << "Finished     "
                     << screen->currentLine << " / " << screen->totalLines << endl;
            }
        }
        Utils::printDivider();

        if (debug) {
            scheduler.debugSchedulerState();
        }
    }
}

int MainConsole::checkCoresUsed()
{
    int coresUsed = 0;
    for (int i = 0; i < scheduler.coresAvailable.size(); i++)
    {
        if (scheduler.coresAvailable[i]->process_to_execute != nullptr)
            coresUsed++;
    }

    return coresUsed;
}


bool MainConsole::checkExistingScreen(const string& processName) {
    for (const auto& screen : screens) {
        if (screen->processName == processName) {
            return true;
        }
    }
    return false;
}

void MainConsole::initScreen(std::shared_ptr<Screen> screen) {
    system("cls");
    screen->printScreen();

    while (true) {
        std::string command;
        cout << Colors::Yellow << "Enter command: " << Colors::White;
        std::getline(std::cin, command);

        if (command == "exit") {
            system("cls");
            Utils::printHeader();
            showConfig = true;
            break;
        }
        else if (command == "clear") {
            system("cls");
            screen->printScreen();
        }
        else if (command == "process-smi") {
            screen->printScreen();
        }
        else {
            cout << Colors::Red << "\nCommand not recognized.\n\n" << Colors::White;
        }
    }
}


void MainConsole::joinAllThreads()
{
    cpuCycleThreadHolder.join();
}

void MainConsole::scheduler_test()
{
    if (freq == 0)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(minCommand, maxCommand); // randomize the amount of commands
        std::uniform_int_distribution<> memory(minMemPerProc, maxMemPerProc);
        int generatedMemory = memory(gen);
        std::shared_ptr<Screen> screen = std::make_shared<Screen>(Screen("process" + std::to_string(currentProcess), 
            currentProcess, 0, dist(gen), Utils::getCurrentTimestamp(), generatedMemory, 
            ceil((generatedMemory * 1.0) / memoryPerFrame)));
        screens.push_back(screen);
        scheduler.readyQueue.push_back(screens.back());

        currentProcess++;
        freq = freqProcess;
        Sleep(200);
    }
    else {
        freq--;
    }
}


void MainConsole::simulateCpuCycle()
{

    while (!hasQuit) {

        // 1. receive new processes
        if (toStartCreatingProcess)
            scheduler_test();

        // 2. run scheduler
        scheduler.run_scheduler();

        // to see if this is an idle cycle, check if all cores have process
        if (isIdleCycle())
            idleCycles++;
        else
            activeCycles++;

        // 3. execute all cores
        for (int i = 0; i < listOfCoreThreads.size(); i++)
            listOfCoreThreads[i] = std::thread(&Core::run_core, scheduler.coresAvailable[i]);

        // wait until cores have finished execution
        for (int i = 0; i < listOfCoreThreads.size(); i++)
            listOfCoreThreads[i].join();

        cpuCycles++;
    }
}


bool MainConsole::isIdleCycle()
{
    for (int i = 0; i < listOfCoreThreads.size(); i++)
    {
        if (scheduler.coresAvailable[i]->process_to_execute != nullptr) // there is a process to be executed
            return false;
    }
    return true;
}


void MainConsole::initialize() {

    ifstream readConfigFile("config.txt");

    if (!readConfigFile.is_open()) {
        cout << "Failed to read the config.txt file." << endl;
    }
    else {
        string cpuOption;
        string configInput;
        std::istringstream iss;

        try {
            // num-cpu
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear(); // clear stream

            if (cpuOption != "num-cpu")
                throw std::invalid_argument("No option for num-cpu");


            nCpuToInitialize = stoi(configInput);
            // to initialize cores after delay-per-exec is received


            // ------------------------ 

            // scheduler
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();// clear stream

            if (cpuOption != "scheduler")
                throw std::invalid_argument("No option for scheduler");

            schedulerType = configInput;




            // ------------------------ 

            // quantum-cycles
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();// clear stream

            if (cpuOption != "quantum-cycles")
                throw std::invalid_argument("No option for quantum-cycles");

            quantumCycles = stoi(configInput);

            // ------------------------ 

            // batch-process-freq
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();// clear stream

            if (cpuOption != "batch-process-freq")
                throw std::invalid_argument("No option for batch-process-freq");

            freqProcess = stoi(configInput);

            // ------------------------
            // min-ins
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();// clear stream

            if (cpuOption != "min-ins")
                throw std::invalid_argument("No option for min-ins");

            minCommand = stoi(configInput);

            // ------------------------

            // max-ins
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();// clear stream

            if (cpuOption != "max-ins")
                throw std::invalid_argument("No option for max-ins");

            maxCommand = stoi(configInput);

            // ------------------------ 

            // delays-per-exec
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();// clear stream

            if (cpuOption != "delays-per-exec")
                throw std::invalid_argument("No option for delays-per-exec");

            delayExecFake = stoi(configInput); 
            delayExec = stoi(configInput) + 1; // + 1 for easier time(?)


            // ------------------------

            // max-overall-mem
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();// clear stream

            if (cpuOption != "max-overall-mem")
                throw std::invalid_argument("No option for max-overall-mem");

            maxMemory = stoi(configInput);


            // ------------------------

            // mem-per-frame
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();// clear stream

            if (cpuOption != "mem-per-frame")
                throw std::invalid_argument("No option for mem-per-frame");

            memoryPerFrame = stoi(configInput);



            // ------------------------
            // 
            // mem-per-proc
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();// clear stream

            if (cpuOption != "min-mem-per-proc")
                throw std::invalid_argument("No option for min-mem-per-proc");

            minMemPerProc = stoi(configInput);



            // ------------------------
            // 
            // mem-per-proc
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();// clear stream

            if (cpuOption != "max-mem-per-proc")
                throw std::invalid_argument("No option for max-mem-per-proc");

            maxMemPerProc = stoi(configInput);



            // ------------------------
            memory = make_shared<Memory>();
            initializeCores(nCpuToInitialize, delayExec, quantumCycles, memory);

            if (maxMemory == memoryPerFrame)
                memory->initialize(maxMemory, memoryPerFrame, 0);
            else
                memory->initialize(maxMemory, memoryPerFrame, 1);

            if (scheduler.initialize_scheduler(schedulerType, memory)) // invalid config return 1
                throw std::invalid_argument("Invalid scheduler option");


            initialized = true;

            Utils::printConfirmation("System initialized.");
            showConfig = true;

            // start cpu cycles
            cpuCycleThreadHolder = std::thread(&MainConsole::simulateCpuCycle, this);

        }
        catch (std::exception& e) {
            cout << "Error in reading config.txt: " << e.what() << endl;
        }
    }
}