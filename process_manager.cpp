#include "process_manager.h"
#include <algorithm>
#include <windows.h>
#include <iostream>
#include <thread>

namespace ProcessManager {
    vector<Process> processes;
    atomic<bool> stopFlag(false);
    int idCounter = 0;
    int maxProcesses = 10;

    string generateRandomName() {
        string name = "process_";
        for (int i = 0; i < 5; ++i) {
            name += to_string(Utils::generateRandomNumber(0, 9));
        }
        return name;
    }

    bool checkProcessExist(const string& name) {
        auto result = find_if(processes.begin(), processes.end(), [name](const Process& obj) {
            return obj.getName() == name;
            });

        return result != processes.end();
    }

    void createProcess(std::shared_ptr<Scheduler>& scheduler, const string& name) {
        if (checkProcessExist(name)) return;
        processes.emplace_back(name, idCounter);
        idCounter++;
        Process& newProcessRef = processes.back();
        scheduler->addProcessToQueue(newProcessRef);  
    }

    Process* findProcess(const string& name) {
        for (auto& process : processes) {
            if (process.getName() == name) return &process;
        }
        return nullptr;
    }

    void redrawProcess(const string& name) {
        if (Process* process = findProcess(name)) {
            if (process->getStatus() != 3) { process->drawConsole(); }
            else { cout << "  Process " << process->getName() << " not found.\n"; }
        }
        else {
            auto showError = []() {
                cout << Colors::Red << "  Process not found! Cannot redraw" << Colors::White << endl;
                };
            runScreenLoop(showError);
        }
    }

    void startProcessGeneration(int frequency, std::shared_ptr<Scheduler>& scheduler, 
                                std::atomic<bool>& stopFlag) {
        int processCount = 0;
        std::vector<Process> processes;
        int count = 0;

        while (!stopFlag) {
            if (processCount < maxProcesses) {
                if (count == 1) { cout << "second"; }
                std::this_thread::sleep_for(std::chrono::milliseconds(frequency));

                std::string newProcessName = generateRandomName();
                processes.emplace_back(newProcessName, idCounter);
                idCounter++;
                Process& newProcessRef = processes.back();

                scheduler->addProcessToQueue(newProcessRef);
                processCount++;
                //std::cout << "Added process #" << processCount << ": " << newProcessName << std::endl;
            }
            else {
                processCount = 0;
                //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            }
        }
    }

    void stopProcessGeneration() {
        stopFlag = true;
    }

    void printProcess(std::string processName, std::string creationTime, int currLine, int totalLine){
        cout << processName << " | ";
        cout << "(" << creationTime << ") | ";
        cout << "Core: N/A | ";
        cout << currLine << " / " << totalLine << " | ";
    }

    void displayProcessList(CPU& cpu) {
        int totalCores = cpu.getNumCores();
        int usedCores = cpu.getUsedCores();
        int availableCores = totalCores - usedCores;

        cout << "\n--------------------------------------" << endl;
        cout << "CPU Utilization: " << (usedCores * 100) / totalCores << "%" << endl;
        cout << "Cores used: " << usedCores << endl;
        cout << "Cores available: " << availableCores << endl;
        cout << "--------------------------------------" << endl;
        cout << "Running processes:" << endl;

        for (const auto& process : processes) {

            if (process.getStatus() == Process::Running)
            {
                cout << process.getName() << " | ";
                cout << "(" << process.getCreationTime() << ") | ";
                cout << "Core: " << process.getCore() << " | ";
                cout << "Status: Running | ";
                cout << process.getCurrentLine() << " / " << process.getTotalLines() << " | " << endl;
            }
        }

        cout << "\nFinished processes:\n";
        cout << "\n--------------------------------------" << endl;

        for (const auto& process : processes) {
            if (process.getStatus() == Process::Done)
            {
                cout << process.getName() << " | ";
                cout << "(" << process.getCreationTime() << ") | ";
                cout << "Finished     ";
                cout << process.getCurrentLine() << " / " << process.getTotalLines() << " |" << endl;
            }
        }
    }

    void saveReportToFile(CPU& cpu, const std::string& filename) {
        int totalCores = cpu.getNumCores();
        int usedCores = cpu.getUsedCores();
        int availableCores = totalCores - usedCores;

        std::ofstream file(filename, std::ios::out);
        if (!file) {
            std::cerr << "Unable to open file for writing: " << filename << std::endl;
            return;
        }

        file << "CPU Utilization: " << (usedCores * 100) / totalCores << "%" << std::endl;
        file << "Cores used: " << usedCores << std::endl;
        file << "Cores available: " << availableCores << std::endl;
        file << "--------------------------------------" << std::endl;
        file << "Running processes:" << std::endl;

        for (const auto& process : ProcessManager::processes) {
            if (process.getStatus() == Process::Running) {
                file << process.getName() << " | ";
                file << "(" << process.getCreationTime() << ") | ";
                file << "Core: " << process.getCore() << " | ";
                file << "Status: Running | ";
                file << process.getCurrentLine() << " / " << process.getTotalLines() << " | " << std::endl;
            }
        }

        file << "\nFinished processes:\n";
        file << "\n--------------------------------------" << std::endl;

        for (const auto& process : ProcessManager::processes) {
            if (process.getStatus() == Process::Done) {
                file << process.getName() << " | ";
                file << "(" << process.getCreationTime() << ") | ";
                file << "Finished     ";
                file << process.getCurrentLine() << " / " << process.getTotalLines() << " |" << std::endl;
            }
        }

        file.close();
        std::cout << "  Report-util successful!\n" << std::endl;
    }

    void ProcessManager::updateProcess(Process& newProcessDetails, int coreId) {
        Process* existingProcess = findProcess(newProcessDetails.getName());
        if (existingProcess != nullptr) {
            existingProcess->updateCurrentLine(newProcessDetails.getCurrentLine());
            existingProcess->updateProcessStatus(newProcessDetails.getStatus());
            existingProcess->setCore(coreId);
        } else {
            Process newProcessCopy(newProcessDetails.getName(), newProcessDetails.getID());

            newProcessCopy.updateCurrentLine(newProcessDetails.getCurrentLine());
            newProcessCopy.updateProcessStatus(newProcessDetails.getStatus());
            newProcessCopy.setCreationTime(newProcessDetails.getCreationTime());
            newProcessCopy.setTotalLines(newProcessDetails.getTotalLines());
            newProcessCopy.setCore(coreId);

            processes.push_back(newProcessCopy);

            //std::cerr << "Process " << newProcessDetails.getName() << " not found.\n";
        }    
    }
}
//void manageProcesses(CPU& cpu, Scheduler& scheduler, atomic<bool>& stopFlag, int frequency) {
//    thread generationThread([&]() {
//        ProcessManager::startProcessGeneration(frequency, scheduler);
//        });
//
//    generationThread.join();
//
//    while (!stopFlag) {
//        scheduler.scheduleProcess(cpu);
//        cpu.runCores();
//        vector<bool> coreStatus = cpu.getCoresStatus();
//        if (scheduler.getProcessQueue().empty() && all_of(coreStatus.begin(), coreStatus.end(), [](bool status) { return !status; })) {
//            stopFlag = true;
//        }
//
//        /*this_thread::sleep_for(chrono::milliseconds(50));*/
//    }
//}