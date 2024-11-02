#pragma once
#ifndef CPU_H
#define CPU_H

#include <vector>
#include "process.h"

class CPU {
private:
    int num_cores;
    vector<bool> coresStatus;
    vector<Process> processInCore;

public:
    CPU(int num_cpu);
    void runCores();
    void completeProcess(int coreId);
    void removeProcess(int coreId); // for roundRobin
    void assignProcessToCore(Process& process, int coreId);
    vector<bool> getCoresStatus() const;
    const int getNumCores() const;
};

#endif

