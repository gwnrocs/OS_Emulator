#include "Console.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>

using namespace std;

Console::Console() : isMainMenu(true) {}

void Console::start() {
    Utils::printHeader();
    string command, option, process_name;

    while (true) {
        if (showConfig) {
            printConfig();
            showConfig = false;
        }

        std::cout << "  Enter a command: ";
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

                std::cout << "  System stopped. Reinitializing..." << std::endl;
            }

            initialize(); // Call the initialize function to reinitialize the system
            if (initialized) {
                std::cout << "  System initialized successfully.\n" << std::endl;
            }
        }

        else if (command == "exit") {
            hasQuit = true;
            joinAllThreads();

            std::cout << "Exiting program... Goodbye!" << std::endl;
            break;
        }

        else if (command.find("screen") == 0) {

            std::istringstream iss(command.substr(6));
            iss >> option >> process_name;

            handleScreenCommand(option, process_name);
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
            fileOPESY << "CPU Utilization: " << std::round(((coresUsed * 1.0) / scheduler.coresAvailable.size()) * 100) << "%" << std::endl;
            fileOPESY << "Cores used: " << coresUsed << std::endl;
            fileOPESY << "Cores available: " << scheduler.coresAvailable.size() - coresUsed << std::endl << std::endl;

            fileOPESY << "--------------------------------------" << std::endl;

            if (screens.empty()) {
                fileOPESY << "No screens attached." << std::endl;
            }
            else {

                fileOPESY << "Running processes: " << std::endl;

                for (const auto& screen : screens) {
                    if (screen->status == Screen::RUNNING)
                    {
                        fileOPESY << screen->process_name << "    ";
                        fileOPESY << "(" + screen->created_at + ")    ";
                        fileOPESY << "Core: " + std::to_string(screen->core_id_assigned) << "    ";
                        fileOPESY << screen->curr_line_instr << " / " << screen->total_line_instr << "\n";

                    }

                }

                fileOPESY << std::endl;
                fileOPESY << "Finished processes: " << std::endl;

                for (const auto& screen : screens) {
                    if (screen->status == Screen::FINISHED)
                    {
                        fileOPESY << screen->process_name << "    ";
                        fileOPESY << "(" + screen->created_at + ")  ";
                        fileOPESY << "Finished     ";
                        fileOPESY << screen->curr_line_instr << " / " << screen->total_line_instr << "\n";
                    }

                }
                fileOPESY << "--------------------------------------" << std::endl;

            }
            fileOPESY.close();

            //
            std::cout << "Successfully printed report-util." << std::endl;
        }
        else if (command == "scheduler -test") {
            if (toStartCreatingProcess)
            {
                std::cout << "scheduler -test is already activated." << std::endl;
            }
            else {
                toStartCreatingProcess = true;
                std::cout << "scheduler -test activated." << std::endl;
            }

        }
        else if (command == "scheduler -stop") {
            if (!toStartCreatingProcess)
                std::cout << "No ongoing scheduler-test at the moment." << std::endl;
            else {
                toStartCreatingProcess = false;
                std::cout << "Stopped scheduler-test." << std::endl;
            }
        }
        else if (command == "process-smi") {
            int coresUsed = checkCoresUsed();
            int memoryUsage = memory->getMemoryUsage();
            std::cout << "-------------------------------------------------------------" << std::endl;
            std::cout << "|                       PROCESS-SMI                         |" << std::endl;
            std::cout << "-------------------------------------------------------------" << std::endl;
            std::cout << "CPU-Util: " << std::round(((coresUsed * 1.0) / scheduler.coresAvailable.size()) * 100) << "%" << std::endl;
            std::cout << "Memory Usage: " << memoryUsage << "KB / " << memory->maxMemory << "KB" << std::endl;
            std::cout << "Memory Util: " << std::round(((memoryUsage * 1.0) / memory->maxMemory) * 100) << "%" << std::endl << std::endl;
            std::cout << "==============================================================" << std::endl;
            std::cout << "Running processes and memory usage:" << std::endl;
            std::cout << "-------------------------------------------------------------" << std::endl;

            for (int i = 0; i < memory->processInMemory.size(); i++)
                std::cout << memory->processInMemory[i]->process_name << " " << memory->processInMemory[i]->memory_to_occupy << "KB" << std::endl;

            std::cout << "-------------------------------------------------------------" << std::endl;



        }
        else if (command == "vmstat") {
            int memoryUsage = memory->getMemoryUsage();
            int freeMemory = memory->maxMemory - memoryUsage;
            std::cout << std::endl;
            std::cout << memory->maxMemory << " total memory" << std::endl;
            std::cout << memoryUsage << " used memory" << std::endl;
            std::cout << freeMemory << " free memory" << std::endl;
            std::cout << idleCycles << " idle CPU ticks" << std::endl;
            std::cout << activeCycles << " active CPU ticks" << std::endl;
            std::cout << cpuCycles << " total CPU ticks" << std::endl;
            std::cout << memory->numPagedIn << " num paged in" << std::endl;
            std::cout << memory->numPagedOut << " num paged out" << std::endl;
            std::cout << std::endl;

        }
        else {
            showConfig = true;
            Utils::printError(command);
        }

    }
}


void Console::initializeCores(int numCores, int delay, int quantumCycles, std::shared_ptr<Memory> memory)
{
    for (int i = 0; i < numCores; i++) {
        auto core = make_shared<Core>(i, delay, quantumCycles, memory);
        scheduler.coresAvailable.push_back(core);

    }

    listOfCoreThreads.resize(numCores); // reserve space in advance
}


void Console::handleScreenCommand(const string& option, const string& process_name) {
    if (option == "-r" && process_name != "") {
        attachScreen(process_name);
    }
    else if (option == "-s" && process_name != "") {
        createScreen(process_name);
    }
    else if (option == "-ls") {
        listScreens();
    }
    // Lists core availability and ready queue along with the normal screen -ls
    else if (option == "-ls-debug") {
        listScreens(true);
    }
    else {
        std::cout << "Command not recognized." << std::endl;
    }
}


void Console::printConfig() {
    cout << Colors::Grey << "  Current Configuration:" << Colors::White << endl;

    std::cout << "  Number of CPUs: " << nCpuToInitialize << std::endl;

    if (schedulerType == "rr") {
        std::cout << "  Scheduler Type: Round Robin" << std::endl;
        std::cout << "  Quantum Cycles: " << quantumCycles << std::endl;
    }
    else {
        std::cout << "  Scheduler Type: First Come First Serve" << std::endl;
        std::cout << "  Quantum Cycles: N/A" << std::endl;
    }

    std::cout << "  Batch Process Frequency: " << freqProcess << std::endl;
    std::cout << "  Minimum Instructions: " << minCommand << std::endl;
    std::cout << "  Maximum Instructions: " << maxCommand << std::endl;
    std::cout << "  Delays Per Execution: " << delayExecFake << std::endl;
    std::cout << "  Maximum Overall Memory: " << maxMemory << " KB" << std::endl;
    std::cout << "  Memory Per Frame: " << memoryPerFrame << " KB" << std::endl;
    std::cout << "  Minimum Memory Per Process: " << minMemPerProc << " KB" << std::endl;
    std::cout << "  Maximum Memory Per Process: " << maxMemPerProc << " KB" << std::endl;

    cout << Colors::Grey << "\n  ---------------------------------- \n" << Colors::White << endl;
}



void Console::createScreen(const string& process_name) {
    if (!checkExistingScreen(process_name)) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(minCommand, maxCommand); // randomize the amount of commands
        std::uniform_int_distribution<> mem(minMemPerProc, maxMemPerProc);
        int generatedMemory = mem(gen);

        int pid = screens.size();
        std::shared_ptr<Screen> screen = make_shared<Screen>(Screen(process_name, pid, 0, dist(gen), 
                                        Utils::getCurrentTimestamp(), generatedMemory, 
                                        ceil((generatedMemory * 1.0) / memoryPerFrame)));
        screens.push_back(screen);
        scheduler.readyQueue.push_back(screen);
        initScreen(screen);
    }
    else {
        std::cout << "Screen initialization failed. Please use another process name." << std::endl;
    }
}

// to check if process has finished, then do not enter initScreen()
void Console::attachScreen(const string& process_name) {
    bool screenFound = false;
    for (int i = 0; i < screens.size(); i++) {
        if (screens[i]->process_name == process_name) {

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
        std::cout << "Process " << process_name << " not found." << std::endl;
    }
}

void Console::listScreens(bool debug) {

    int coresUsed = checkCoresUsed();
    std::cout << "CPU Utilization: " << std::round(((coresUsed * 1.0) / scheduler.coresAvailable.size()) * 100) << "%" << std::endl;
    std::cout << "Cores used: " << coresUsed << std::endl;
    std::cout << "Cores available: " << scheduler.coresAvailable.size() - coresUsed << std::endl << std::endl;

    std::cout << "--------------------------------------" << std::endl;

    if (screens.empty()) {
        std::cout << "No screens attached." << std::endl;
    }
    else {

        std::cout << "Running processes: " << std::endl;

        for (const auto& screen : screens) {
            if (screen->status == Screen::RUNNING)
            {
                std::cout << screen->process_name << "    ";
                std::cout << "(" + screen->created_at + ")    ";
                std::cout << "Core: " + std::to_string(screen->core_id_assigned) << "    ";
                std::cout << screen->curr_line_instr << " / " << screen->total_line_instr << "\n";

            }

        }

        std::cout << std::endl;
        std::cout << "Finished processes: " << std::endl;

        for (const auto& screen : screens) {
            if (screen->status == Screen::FINISHED)
            {
                std::cout << screen->process_name << "    ";
                std::cout << "(" + screen->created_at + ")  ";
                std::cout << "Finished     ";
                std::cout << screen->curr_line_instr << " / " << screen->total_line_instr << "\n";
            }


        }
        std::cout << "--------------------------------------" << std::endl;

        // For calling screen -ls-debug
        if (debug) {
            scheduler.debugSchedulerState();
        }
    }
}

int Console::checkCoresUsed()
{
    int coresUsed = 0;
    for (int i = 0; i < scheduler.coresAvailable.size(); i++)
    {
        if (scheduler.coresAvailable[i]->process_to_execute != nullptr)
            coresUsed++;
    }

    return coresUsed;

}


bool Console::checkExistingScreen(const string& process_name) {
    for (const auto& screen : screens) {
        if (screen->process_name == process_name) {
            return true;
        }
    }
    return false;
}

void Console::initScreen(std::shared_ptr<Screen> screen) {
    system("cls");
    screen->printScreen();

    while (true) {
        std::string command;
        std::cout << "root:/> ";
        std::getline(std::cin, command);

        if (command == "exit") {
            Utils::clearScreen();
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
            std::cout << "Unknown command. Please try again." << std::endl;
        }
    }
}



void Console::joinAllThreads()
{
    cpuCycleThreadHolder.join();
}

void Console::scheduler_test()
{
    // we made it -1 in instantiating batch-per-freq, so it's centered at 0
    if (freq == 0) // if statement will dictate if it will create a process
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


void Console::simulateCpuCycle()
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


bool Console::isIdleCycle()
{
    for (int i = 0; i < listOfCoreThreads.size(); i++)
    {
        if (scheduler.coresAvailable[i]->process_to_execute != nullptr) // there is a process to be executed
            return false;
    }
    return true;
}


void Console::initialize() {

    ifstream readConfigFile("config.txt");

    if (!readConfigFile.is_open()) {
        std::cout << "Failed to read the config.txt file." << std::endl;
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
            cpuCycleThreadHolder = std::thread(&Console::simulateCpuCycle, this);

        }
        catch (std::exception& e) {
            std::cout << "Error in reading config.txt: " << e.what() << std::endl;
        }
    }
}