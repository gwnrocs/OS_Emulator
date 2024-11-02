#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "cpu.h"
#include "process.h"
#include "utils.h"
#include <queue>
#include <string>

using namespace std;

struct ExecutionRecord {
    std::string processName;
    int coreIndex;
    int startTime;
    int endTime;
    std::string updateFrom;
};

class Scheduler {
public:
    void addProcessToQueue(Process& process);
    void scheduleProcess(CPU& cpu);
    void FCFS(CPU& cpu);
    queue<Process> getProcessQueue();
    void RoundRobin(CPU& cpu, int quantum_cycles);
    void printProcessQueue();

private:
    queue<Process> processQueue;
    atomic<int> currentQuantum;
};

#endif