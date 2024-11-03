#include "scheduler.h"
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <thread>

using namespace std;


void outGanttChart(vector<vector<ExecutionRecord>> ganttChartData, string processQueueString){
    
    ofstream outFile("gantt_chart.txt", std::ios::app);  // Open file in append mode
    if (outFile.is_open()) {
        outFile << processQueueString;
        for (int coreId = 0; coreId < ganttChartData.size(); ++coreId) {
            outFile << "Gantt Chart for Core " << coreId << ":\n";
            for (const auto& record : ganttChartData[coreId]) {
                outFile << "Core " << record.coreIndex << ": [" << record.startTime << "-" << record.endTime
                  << "] Process: " << record.processName << endl;
            };
            outFile << "\n";
        }
        outFile.close();
    } else {
        cerr << "Unable to open file for writing the Gantt chart data.\n";
    }
}
void displayGanttChart(const vector<ExecutionRecord>& data) {
    for (const auto& record : data) {
        cout << "Core " << record.coreIndex << ": [" << record.startTime << "-" << record.endTime
                  << "] Process: " << record.processName << endl;
    }
}

void Scheduler::addProcessToQueue(Process& process) {
    process.updateProcessStatus(Process::Waiting);
    processQueue.push(process);
}

void Scheduler::scheduleProcess(CPU& cpu) {
    int num_cpu, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec;
    string scheduler_type;

    loadConfig(num_cpu, scheduler_type, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec);
    vector<vector<ExecutionRecord>> ganttChartData(num_cpu);

    string processQueueString = printProcessQueue();
    // cout << processQueueString << endl;

    if (scheduler_type == "fcfs") {
        FCFS(cpu);
    } else if (scheduler_type == "rr") {
        ganttChartData = RoundRobin(cpu, quantum_cycles);
    }

    outGanttChart(ganttChartData, processQueueString);
    
}

void Scheduler::FCFS(CPU& cpu) {
    for (int i = 0; i < cpu.getNumCores(); ++i) {
        if (!processQueue.empty() && !cpu.getCoresStatus()[i]) {
            Process process = processQueue.front();
            processQueue.pop();
            cpu.assignProcessToCore(process, i);
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



vector<vector<ExecutionRecord>> Scheduler::RoundRobin(CPU& cpu, int quantum_cycles) {
    int currentTime = 0;
    vector<vector<ExecutionRecord>> ganttChartData(cpu.getNumCores());
    vector<int> quantumCycleCounters(cpu.getNumCores(), 0);

    while (!processQueue.empty() || !cpu.areAllCoresIdle()) {
        assignProcessesToIdleCores(cpu, ganttChartData, quantumCycleCounters, currentTime);
        
        for (int cycle = 0; cycle < quantum_cycles; ++cycle) {
            ++currentTime;
            updateCoreExecutions(cpu, ganttChartData, quantumCycleCounters, currentTime, quantum_cycles);
        }
    }

    return ganttChartData;
}

void Scheduler::assignProcessesToIdleCores(CPU& cpu, vector<vector<ExecutionRecord>>& ganttChartData, 
                                           vector<int>& quantumCycleCounters, int currentTime) {
    for (int i = 0; i < cpu.getNumCores(); ++i) {
        if (!cpu.getCoresStatus()[i] && !processQueue.empty()) {
            Process currentProcess = processQueue.front();
            processQueue.pop();
            cpu.assignProcessToCore(currentProcess, i);
            currentProcess.updateProcessStatus(Process::Running);

            ganttChartData[i].push_back({currentProcess.getName(), i, currentTime, -1});
            quantumCycleCounters[i] = 0;
        }
    }
}

void Scheduler::updateCoreExecutions(CPU& cpu, vector<vector<ExecutionRecord>>& ganttChartData, 
                                     vector<int>& quantumCycleCounters, int currentTime, int quantum_cycles) {
    for (int coreId = 0; coreId < cpu.getNumCores(); ++coreId) {
        if (cpu.getCoresStatus()[coreId]) {
            Process& runningProcess = cpu.getProcessOnCore(coreId);
            runningProcess.executeInstruction();
            ganttChartData[coreId].back().endTime = currentTime;
            ++quantumCycleCounters[coreId];

            if (runningProcess.getStatus() == Process::Done) {
                handleProcessCompletion(cpu, coreId, ganttChartData, quantumCycleCounters, currentTime);
            } else if (quantumCycleCounters[coreId] >= quantum_cycles) {
                handleQuantumExpiration(cpu, coreId, ganttChartData, quantumCycleCounters, currentTime);
            }
        }
    }
}

void Scheduler::handleProcessCompletion(CPU& cpu, int coreId, vector<vector<ExecutionRecord>>& ganttChartData, 
                                        vector<int>& quantumCycleCounters, int currentTime) {
    cpu.completeProcess(coreId);
    quantumCycleCounters[coreId] = 0;
    if (!processQueue.empty()) {
        Process currentProcess = processQueue.front();
        processQueue.pop();
        cpu.assignProcessToCore(currentProcess, coreId);
        ganttChartData[coreId].push_back({ currentProcess.getName(), coreId, currentTime, -1 });
    }
}

void Scheduler::handleQuantumExpiration(CPU& cpu, int coreId, vector<vector<ExecutionRecord>>& ganttChartData, 
                                        vector<int>& quantumCycleCounters, int currentTime) {
    Process& runningProcess = cpu.getProcessOnCore(coreId);
    processQueue.push(runningProcess);
    runningProcess.updateProcessStatus(Process::Waiting);
    cpu.removeProcess(coreId);
    quantumCycleCounters[coreId] = 0;

    if (!processQueue.empty()) {
        Process currentProcess = processQueue.front();
        processQueue.pop();
        cpu.assignProcessToCore(currentProcess, coreId);
        ganttChartData[coreId].push_back({ currentProcess.getName(), coreId, currentTime, -1 });
    }
}



queue<Process> Scheduler::getProcessQueue() {
    return processQueue;
}
