#pragma once
#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <vector>
#include <atomic>
#include "scheduler.h"
#include "runScreenLoop.h"

namespace ProcessManager {
    extern vector<Process> processes;
    extern atomic<bool> stopFlag;
    extern int maxProcesses;

    string generateRandomName();
    bool checkProcessExist(const string& name);
    void createProcess(std::shared_ptr<Scheduler>& scheduler, const string& name);
    Process* findProcess(const string& name);
    void redrawProcess(const string& name);
    void startProcessGeneration(int frequency, std::shared_ptr<Scheduler>& scheduler, std::atomic<bool>& stopFlag);
    void stopProcessGeneration();
    void displayProcessList(CPU& cpu);
    void saveReportToFile(CPU& cpu, const std::string& filename);
    void updateProcess(Process& process, int core);
}

//void manageProcesses(CPU& cpu, Scheduler& scheduler, atomic<bool>& stopFlag, int frequency);

#endif