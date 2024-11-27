#include "MainConsole.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>

using namespace std;

MainConsole::MainConsole() : isMainMenu(true) {}

void MainConsole::drawConsole() {
    Utils::printHeader();
    string command, option, processName;
    isRunning = true;

    while (isRunning) {
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
                joinAllThreads();

                // Reset all states
                screens.clear();
                scheduler.readyQueue.clear();
                scheduler.coresAvailable.clear();
                listOfCoreThreads.clear();

                cout << "System stopped. Reinitializing..." << endl;
            }

            initialize(); // Call the initialize function to reinitialize the system
            if (initialized) {
                cout << "System initialized successfully." << endl << endl;
            }
        }

        else if (command == "exit") {
            isRunning = false;
            joinAllThreads();

            cout << Colors::Red << "\nExiting program . . . " << Colors::White << endl;
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
            generateReportUtil();
            cout << Colors::Green << "\nSuccessfully printed report-util (csopesy-log.txt)\n" << Colors::White << endl;
        }
        else if (command == "scheduler -test") {
            if (allowProcessCreation)
                cout << Colors::Green << "\nProcess generation is already activated . . .\n\n" << Colors::White;
            else 
                allowProcessCreation = true;
                cout << Colors::Green << "\nProcess generation has started . . .\n\n" << Colors::White;
        }
        else if (command == "scheduler -stop") {
            if (!allowProcessCreation)
                cout << Colors::Red << "\nProcess generation is not activated . . .\n\n" << Colors::White;
            else {
                allowProcessCreation = false;
                cout << Colors::Red << "\nProcess generation has stopped . . .\n\n" << Colors::White;
            }
        }

        else if (command == "process-smi") {
            generateProcessSMI();
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

void MainConsole::initialize() {
    ifstream readConfigFile("config.txt");

    if (!readConfigFile.is_open()) {
        cout << "Unable to open config.txt file." << endl;
    }
    else {
        string cpuOption, configInput;
        std::istringstream iss;

        try {
            // Read number of CPUs
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();

            if (cpuOption != "num-cpu") {
                throw std::invalid_argument("Missing num-cpu option");
            }
            nCpuToInitialize = stoi(configInput);

            // Read scheduler type
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();

            if (cpuOption != "scheduler") {
                throw std::invalid_argument("Missing scheduler option");
            }
            schedulerType = configInput;

            // Read quantum cycles
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();

            if (cpuOption != "quantum-cycles") {
                throw std::invalid_argument("Missing quantum-cycles option");
            }
            quantumCycles = stoi(configInput);

            // Read batch process frequency
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();

            if (cpuOption != "batch-process-freq") {
                throw std::invalid_argument("Missing batch-process-freq option");
            }
            freqProcess = stoi(configInput);

            // Read minimum instructions
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();

            if (cpuOption != "min-ins") {
                throw std::invalid_argument("Missing min-ins option");
            }
            minCommand = stoi(configInput);

            // Read maximum instructions
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();

            if (cpuOption != "max-ins") {
                throw std::invalid_argument("Missing max-ins option");
            }
            maxCommand = stoi(configInput);

            // Read delays per execution
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();

            if (cpuOption != "delays-per-exec") {
                throw std::invalid_argument("Missing delays-per-exec option");
            }
            delayExecFake = stoi(configInput);
            delayExec = stoi(configInput) + 1;

            // Read maximum overall memory
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();

            if (cpuOption != "max-overall-mem") {
                throw std::invalid_argument("Missing max-overall-mem option");
            }
            maxMemory = stoi(configInput);

            // Read memory per frame
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();

            if (cpuOption != "mem-per-frame") {
                throw std::invalid_argument("Missing mem-per-frame option");
            }
            memoryPerFrame = stoi(configInput);

            // Read minimum memory per process
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();

            if (cpuOption != "min-mem-per-proc") {
                throw std::invalid_argument("Missing min-mem-per-proc option");
            }
            minMemPerProc = stoi(configInput);

            // Read maximum memory per process
            getline(readConfigFile, cpuOption);
            iss.str(cpuOption);
            iss >> cpuOption >> configInput;
            iss.clear();

            if (cpuOption != "max-mem-per-proc") {
                throw std::invalid_argument("Missing max-mem-per-proc option");
            }
            maxMemPerProc = stoi(configInput);

            // Initialize memory and cores
            memory = make_shared<Memory>();
            initializeCores(nCpuToInitialize, delayExec, quantumCycles, memory);

            if (maxMemory == memoryPerFrame) {
                memory->initialize(maxMemory, memoryPerFrame, 0);
            }
            else {
                memory->initialize(maxMemory, memoryPerFrame, 1);
            }

            if (scheduler.initializeScheduler(schedulerType, memory)) {
                throw std::invalid_argument("Invalid scheduler configuration");
            }

            initialized = true;
            Utils::printConfirmation("System initialized.");
            showConfig = true;

            // Start CPU simulation thread
            cpuCycleThreadHolder = std::thread(&MainConsole::simulateCpuCycle, this);

        }
        catch (const std::exception& e) {
            cout << "Error reading config.txt: " << e.what() << endl;
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

            if (screens[i]->status == Screen::DONE)
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
            if (screen->status == Screen::DONE)
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

void MainConsole::schedulerTest()
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
    while (isRunning) {

        if (allowProcessCreation)
            schedulerTest();

        scheduler.startScheduler();

        if (isIdleCycle())
            idleCycles++;
        else
            activeCycles++;

        for (int i = 0; i < listOfCoreThreads.size(); i++)
            listOfCoreThreads[i] = std::thread(&Core::run_core, scheduler.coresAvailable[i]);

        for (int i = 0; i < listOfCoreThreads.size(); i++)
            listOfCoreThreads[i].join();

        cpuCycles++;
    }
}


bool MainConsole::isIdleCycle()
{
    for (int i = 0; i < listOfCoreThreads.size(); i++)
        if (scheduler.coresAvailable[i]->process_to_execute != nullptr)
            return false;
  
    return true;
}

void MainConsole::generateReportUtil() {
    ofstream fileOPESY;
    fileOPESY.open("csopesy-log.txt");

    // Write CPU utilization details
    int coresUsed = checkCoresUsed();
    fileOPESY << "--------------------------------------" << endl
        << "CPU Utilization: " << std::round(((coresUsed * 1.0) / scheduler.coresAvailable.size()) * 100) << "%" << endl
        << "Cores used: " << coresUsed << endl
        << "Cores available: " << scheduler.coresAvailable.size() - coresUsed << endl << endl
        << "--------------------------------------" << endl;

    // Write screen details
    if (screens.empty()) {
        fileOPESY << "No screens attached." << endl;
    }
    else {
        fileOPESY << "Running processes: " << endl;
        for (const auto& screen : screens) {
            if (screen->status == Screen::RUNNING) {
                fileOPESY << screen->processName << "    "
                    << "(" + screen->creationTime + ")    "
                    << "Core: " + std::to_string(screen->core_id_assigned) << "    "
                    << screen->currentLine << " / " << screen->totalLines << "\n";
            }
        }

        fileOPESY << endl;
        fileOPESY << "Finished processes: " << endl;
        for (const auto& screen : screens) {
            if (screen->status == Screen::DONE) {
                fileOPESY << screen->processName << "    "
                    << "(" + screen->creationTime + ")  "
                    << "Finished     "
                    << screen->currentLine << " / " << screen->totalLines << "\n";
            }
        }
        fileOPESY << "--------------------------------------" << endl;
    }

    fileOPESY.close();
}

void MainConsole::generateProcessSMI() {
    int coresUsed = checkCoresUsed();
    int memoryUsage = memory->getMemoryUsage();
    int memoryUtilization = std::round(((memoryUsage * 1.0) / memory->maxMemory) * 100);
    int cpuUtilization = std::round(((coresUsed * 1.0) / scheduler.coresAvailable.size()) * 100);

    // Header
    cout << endl 
         << "-------------------------------------------------------------" << endl
         << "|                       PROCESS-SMI                         |" << endl
         << "-------------------------------------------------------------" << endl;

    // System Utilization
    cout << "CPU Utilization:     " << cpuUtilization << "%" << endl;
    cout << "Memory Usage:        " << memoryUsage << "KB / " << memory->maxMemory << "KB" << endl;
    cout << "Memory Utilization:  " << memoryUtilization << "%" << endl;
    cout << "-------------------------------------------------------------" << endl;

    // Running Processes
    if (memory->processInMemory.empty()) {
        cout << "No running processes." << endl;
    }
    else {
        cout << "Running Processes and Memory Usage:" << endl;
        cout << "-------------------------------------------------------------" << endl;
        for (const auto& process : memory->processInMemory) {
            cout << process->processName << " - Memory: " << process->memory_to_occupy << "KB" << endl;
        }
        cout << "-------------------------------------------------------------" << endl;
    }

    // Footer
    cout << "=============================================================" << endl;
}