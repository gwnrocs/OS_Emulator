#include "process_manager.h"
#include <algorithm>
#include <windows.h>
#include <iostream>
#include <thread>

namespace ProcessManager {
    vector<Process> processes;
    atomic<bool> stopFlag(false);
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

    void createProcess(const string& name) {
        if (checkProcessExist(name)) return;
        Process newProcess(name);
        processes.push_back(newProcess);
        newProcess.drawConsole();
    }

    Process* findProcess(const string& name) {
        for (auto& process : processes) {
            if (process.getName() == name) return &process;
        }
        return nullptr;
    }

    void redrawProcess(const string& name) {
        if (Process* process = findProcess(name)) {
            process->drawConsole();
        }
        else {
            auto showError = []() {
                cout << Colors::Red << "  Process not found! Cannot redraw" << Colors::White << endl;
                };
            runScreenLoop(showError);
        }
    }

    void startProcessGeneration(int frequency, Scheduler& scheduler) {
        int processCount = 0;
        while (processCount < maxProcesses && !stopFlag) {
            Sleep(frequency * 1);
            string newProcessName = generateRandomName();
            Process newProcess(newProcessName);
            scheduler.addProcessToQueue(newProcess);
            processes.push_back(newProcess);
            processCount++;
        }
    }

    void stopProcessGeneration() {
        stopFlag = true;
    }

    void displayProcessList(CPU& cpu) {
        int totalCores = cpu.getNumCores();
        int usedCores = 0;
        int availableCores = totalCores - usedCores;

        auto showProcesses = [&]() {
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
                    cout << "Core: N/A | ";
                    cout << process.getCurrentLine() << " / " << process.getTotalLines() << " |" << endl;
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
            };

        runScreenLoop(showProcesses);
    }
}

void manageProcesses(CPU& cpu, Scheduler& scheduler, atomic<bool>& stopFlag, int frequency) {
    thread generationThread([&]() {
        ProcessManager::startProcessGeneration(frequency, scheduler);
        });

    generationThread.join();

    while (!stopFlag) {
        scheduler.scheduleProcess(cpu);
        cpu.runCores();
        vector<bool> coreStatus = cpu.getCoresStatus();
        if (scheduler.getProcessQueue().empty() && all_of(coreStatus.begin(), coreStatus.end(), [](bool status) { return !status; })) {
            stopFlag = true;
        }

        /*this_thread::sleep_for(chrono::milliseconds(50));*/
    }

    
}