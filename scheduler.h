#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "cpu.h"
#include "process.h"
#include "utils.h"
#include <queue>
#include <string>
#include <vector>

using namespace std;

class Scheduler {
public:
    void addProcessToQueue(Process& process);
    void scheduleProcess(CPU& cpu);
    void FCFS(CPU& cpu);
    queue<Process> getProcessQueue();
    void RoundRobin(CPU& cpu, int quantum_cycles);
    void assignProcessesToIdleCores(CPU& cpu, int currentTime);
    void handleProcessCompletion(CPU& cpu, int coreId, vector<int>& quantumCycleCounters, int currentTime);
    void handleQuantumExpiration(CPU& cpu, int coreId, vector<int>& quantumCycleCounters, int currentTime);
    
    void runScheduler(CPU& cpu);


    string printProcessQueue();

private:
    queue<Process> processQueue;
    atomic<int> currentQuantum;
};

#endif