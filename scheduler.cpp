#include "scheduler.h"
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <thread>

using namespace std;


void outGanttChart(vector<vector<ExecutionRecord>> ganttChartData, string processQueueString){
    
    ofstream outFile("gantt_chart.txt");
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

    if (scheduler_type == "fcfs") {
        FCFS(cpu);
    } else if (scheduler_type == "rr") {
        ganttChartData = RoundRobin(cpu, quantum_cycles);
    }

    outGanttChart(ganttChartData, processQueueString);
    
}

void Scheduler::FCFS(CPU& cpu) {
    printProcessQueue();
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
        for (int i = 0; i < cpu.getNumCores(); ++i) {
            if (!cpu.getCoresStatus()[i] && !processQueue.empty()) {
                Process current_process = processQueue.front();
                processQueue.pop();
                cpu.assignProcessToCore(current_process, i);
                current_process.updateProcessStatus(Process::Running);

                ganttChartData[i].push_back({current_process.getName(), i, currentTime, -1});
                
                quantumCycleCounters[i] = 0;
            }
        }

        for (int i = 0; i < quantum_cycles; ++i) {
            ++currentTime;

            for (int coreId = 0; coreId < cpu.getNumCores(); ++coreId) {
                if (cpu.getCoresStatus()[coreId]) {
                    Process& runningProcess = cpu.getProcessOnCore(coreId);
                    runningProcess.executeInstruction();
                    
                    ganttChartData[coreId].back().endTime = currentTime; 
                    ++quantumCycleCounters[coreId];

                    
                    // runningProcess.printHelloWorld(coreId); 


                    if (runningProcess.getStatus() == Process::Done) {
                        cpu.completeProcess(coreId);
                        quantumCycleCounters[coreId] = 0;
                        if (!processQueue.empty()) {
                            Process current_process = processQueue.front();
                            processQueue.pop();
                            cpu.assignProcessToCore(current_process, coreId);
                            ganttChartData[coreId].push_back({ current_process.getName(), coreId, currentTime, -1 });
                        }


                    } else if (quantumCycleCounters[coreId] >= quantum_cycles) {
                        // If quantum is exceeded, push the process back to the queue
                        processQueue.push(runningProcess);
                        runningProcess.updateProcessStatus(Process::Waiting);
                        cpu.removeProcess(coreId);
                        quantumCycleCounters[coreId] = 0; 

                        if (!processQueue.empty()) {
                            Process current_process = processQueue.front();
                            processQueue.pop();
                            cpu.assignProcessToCore(current_process, coreId);
                            ganttChartData[coreId].push_back({ current_process.getName(), coreId, currentTime, -1});
                        }


                    }
                }
            }
        }
    }

    return ganttChartData;
}



queue<Process> Scheduler::getProcessQueue() {
    return processQueue;
}
