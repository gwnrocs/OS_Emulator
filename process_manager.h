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
    void createProcess(const string& name);
    Process* findProcess(const string& name);
    void redrawProcess(const string& name);
    void startProcessGeneration(int frequency, Scheduler& scheduler);
    void stopProcessGeneration();
    void displayProcessList(CPU& cpu);
}

void manageProcesses(CPU& cpu, Scheduler& scheduler, atomic<bool>& stopFlag, int frequency);

#endif
