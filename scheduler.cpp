#include "scheduler.h"
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <thread>
#include <chrono>
#include "process_manager.h"
#include "ConfigManager.h"
using Config::configParams;


using namespace std;

void Scheduler::runScheduler(CPU& cpu) {
    while (true) {
        //cout << "Scheduler is running in the background...\n";
        scheduleProcess(cpu);
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

void Scheduler::addProcessToQueue(Process& process) {
    process.updateProcessStatus(Process::Waiting);
    processQueue.push(process);
}

void Scheduler::scheduleProcess(CPU& cpu) {
    string processQueueString = printProcessQueue();

    if (configParams.scheduler_type == "fcfs") {
        FCFS(cpu);
    } else if (configParams.scheduler_type == "rr") {
        RoundRobin(cpu, configParams.quantum_cycles);
    }

    cpu.runCores();
    vector<bool> coreStatus = cpu.getCoresStatus();
}

void Scheduler::FCFS(CPU& cpu) {
    for (int i = 0; i < cpu.getNumCores(); ++i) {
        if (!processQueue.empty() && !cpu.getCoresStatus()[i]) {
            Process process = processQueue.front();
            processQueue.pop();
            cpu.assignProcessToCore(process, i);
            ProcessManager::updateProcess(process, i); // screen -ls temp fix

        }
    }
}

string Scheduler::printProcessQueue() {
    queue<Process> tmp_q = processQueue;
    ostringstream output;

    if (tmp_q.empty()) {
        return "Process queue is empty.\n";
    }

    output << "Current Process Queue:\n";
    string totalLinesOutput;
    char processLabel = 'A';

    while (!tmp_q.empty()) {
        Process q_element = tmp_q.front();
        output << "Process " << processLabel << ": "
               << " Name: " << q_element.getName()
               << " | Total Lines: " << q_element.getTotalLines()
               << "\n";

        totalLinesOutput += to_string(q_element.getTotalLines()) + " ";
        tmp_q.pop();
        processLabel++;
    }

    output << "Burst Times: " << totalLinesOutput << "\n";
    return output.str();
}

void Scheduler::RoundRobin(CPU& cpu, int quantum_cycles) {
    int currentTime = 0;
    vector<int> quantumCycleCounters(cpu.getNumCores(), 0);

    while (!processQueue.empty() || !cpu.areAllCoresIdle()) {
        assignProcessesToIdleCores(cpu, currentTime);
        currentTime++;  

        cpu.runCores();

        for (int i = 0; i < cpu.getNumCores(); ++i) {
            if (cpu.getCoresStatus()[i]) {
                quantumCycleCounters[i]++;

                Process& runningProcess = cpu.getProcessOnCore(i);
                if (runningProcess.getStatus() == Process::Done) {
                    ProcessManager::updateProcess(runningProcess, i); // screen -ls temp fix
                    handleProcessCompletion(cpu, i, quantumCycleCounters, currentTime);

                } else if (quantumCycleCounters[i] >= quantum_cycles) {                    
                    handleQuantumExpiration(cpu, i, quantumCycleCounters, currentTime);
                }
            }
        }
    }

}

void Scheduler::assignProcessesToIdleCores( CPU& cpu, 
                                            int currentTime) {
    for (int i = 0; i < cpu.getNumCores(); ++i) {
        if (!cpu.getCoresStatus()[i] && !processQueue.empty()) {
            Process currentProcess = processQueue.front();
            processQueue.pop();
            cpu.assignProcessToCore(currentProcess, i);
            currentProcess.updateProcessStatus(Process::Running);
        }
    }
}

void Scheduler::handleProcessCompletion(CPU& cpu, int coreId,
                                        vector<int>& quantumCycleCounters, 
                                        int currentTime) {
    cpu.completeProcess(coreId);
    quantumCycleCounters[coreId] = 0;
    if (!processQueue.empty()) {
        Process currentProcess = processQueue.front();
        processQueue.pop();
        cpu.assignProcessToCore(currentProcess, coreId);
    }
}

void Scheduler::handleQuantumExpiration(CPU& cpu, int coreId,
                                        vector<int>& quantumCycleCounters, int currentTime) {
    Process& runningProcess = cpu.getProcessOnCore(coreId);
    processQueue.push(runningProcess);
    cpu.removeProcess(coreId);
    quantumCycleCounters[coreId] = 0;

    if (!processQueue.empty()) {
        Process currentProcess = processQueue.front();
        processQueue.pop();
        cpu.assignProcessToCore(currentProcess, coreId);
    }
}

queue<Process> Scheduler::getProcessQueue() {
    return processQueue;
}