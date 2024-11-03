#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "cpu.h"
#include "process.h"
#include "utils.h"
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct ExecutionRecord {
    std::string processName;
    int coreIndex;
    int startTime;
    int endTime;
};

class Scheduler {
public:
    void addProcessToQueue(Process& process);
    void scheduleProcess(CPU& cpu);
    void FCFS(CPU& cpu);
    queue<Process> getProcessQueue();
    vector<vector<ExecutionRecord>> RoundRobin(CPU& cpu, int quantum_cycles);
    void assignProcessesToIdleCores(CPU& cpu, vector<vector<ExecutionRecord>>& ganttChartData, 
                                     vector<int>& quantumCycleCounters, int currentTime);
    void updateCoreExecutions(CPU& cpu, vector<vector<ExecutionRecord>>& ganttChartData, 
                               vector<int>& quantumCycleCounters, int currentTime, int quantum_cycles);
    void handleProcessCompletion(CPU& cpu, int coreId, vector<vector<ExecutionRecord>>& ganttChartData, 
                                 vector<int>& quantumCycleCounters, int currentTime);
    void handleQuantumExpiration(CPU& cpu, int coreId, vector<vector<ExecutionRecord>>& ganttChartData, 
                                 vector<int>& quantumCycleCounters, int currentTime);

    string printProcessQueue();

private:
    queue<Process> processQueue;
    atomic<int> currentQuantum;
};

#endif