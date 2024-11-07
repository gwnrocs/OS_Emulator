#pragma once
#ifndef CPU_H
#define CPU_H

#include <vector>
#include <string>
#include "process.h"
#include "ConfigManager.h"
using Config::configParams;

class CPU {
private:
    vector<bool> coresStatus;
    vector<Process> processInCore;
    int usedCores;
    

public:
    CPU();
    void runCores();
    void completeProcess(int coreId);
    void removeProcess(int coreId); // for roundRobin
    void assignProcessToCore(Process& process, int coreId);
    vector<bool> getCoresStatus() const;
    const int getNumCores() const;
    Process& getProcessOnCore(int coreId); 
    bool areAllCoresIdle() const;
    int getUsedCores() const;

};

#endif