#include "scheduler.h"
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <vector>

using namespace std;

void displayGanttChart(const std::vector<ExecutionRecord>& data) {
    for (const auto& record : data) {
        std::cout << "Core " << record.coreIndex << ": [" << record.startTime << "-" << record.endTime
                  << "] Process: " << record.processName << " Update from: " << record.updateFrom << std::endl;
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

    if (scheduler_type == "fcfs") {
        FCFS(cpu);
    } else if (scheduler_type == "rr") {
        RoundRobin(cpu, quantum_cycles);
    }
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


void Scheduler::printProcessQueue() {
    std::queue<Process> tmp_q = processQueue;  // Create a copy of the queue

    if (tmp_q.empty()) {
        std::cout << "Process queue is empty." << std::endl;
        return;
    }

    std::cout << "Current Process Queue:" << std::endl;
    std::string totalLinesOutput;  // Accumulate total lines for single-line output
    char processLabel = 'A';  // Start labeling processes from 'A'
    
    while (!tmp_q.empty()) {
        Process q_element = tmp_q.front();
        
        // Original detailed output for each process with its label
        std::cout << "Process " << processLabel << ": "
                  << " Name: " << q_element.getName()
                  << " | Total Lines: " << q_element.getTotalLines()
                  << " | Address: " << &q_element
                  << std::endl;
        
        // Add to the single-line output for Total Lines
        totalLinesOutput += std::to_string(q_element.getTotalLines()) + " ";
        tmp_q.pop();
        processLabel++;  // Move to the next letter
    }

    // Print all Total Lines in one line at the end
    std::cout << "Total Lines in Queue: " << totalLinesOutput << std::endl;
}


void Scheduler::RoundRobin(CPU& cpu, int quantum_cycles) {
    int currentTime = 0;
    std::vector<std::vector<ExecutionRecord>> ganttChartData(cpu.getNumCores());  // Separate data for each core
    printProcessQueue();

    std::vector<int> quantumCycleCounters(cpu.getNumCores(), 0);

    while (!processQueue.empty() || !cpu.areAllCoresIdle()) {
        for (int i = 0; i < cpu.getNumCores(); ++i) {
            if (!cpu.getCoresStatus()[i] && !processQueue.empty()) {
                Process current_process = processQueue.front();
                processQueue.pop();
                cpu.assignProcessToCore(current_process, i);
                current_process.updateProcessStatus(Process::Running);

                // Record the start time for Gantt chart for the specific core
                ganttChartData[i].push_back({current_process.getName(), i, currentTime, -1, "top"});
                
                // Reset quantum cycle counter for the newly assigned process
                quantumCycleCounters[i] = 0;
            }
        }

        // Execute one quantum cycle for all processes currently on cores
        for (int i = 0; i < quantum_cycles; ++i) {
            ++currentTime;

            for (int coreId = 0; coreId < cpu.getNumCores(); ++coreId) {
                if (cpu.getCoresStatus()[coreId]) {
                    Process& runningProcess = cpu.getProcessOnCore(coreId);
                    runningProcess.executeInstruction();
                    
                    ganttChartData[coreId].back().endTime = currentTime;  // End time for this core's process
                    
                    // Increment quantum cycle counter for this core
                    ++quantumCycleCounters[coreId];

                    if (runningProcess.getStatus() == Process::Done) {
                        cout << runningProcess.getName() << " done at " << currentTime << endl;
                        cpu.completeProcess(coreId);
                        quantumCycleCounters[coreId] = 0;
                        if (!processQueue.empty()) {
                            Process current_process = processQueue.front();
                            processQueue.pop();
                            cpu.assignProcessToCore(current_process, coreId);
                            ganttChartData[coreId].push_back({ current_process.getName(), coreId, currentTime, -1, "immediate" });
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


                            ganttChartData[coreId].push_back({ current_process.getName(), coreId, currentTime, -1, "immediate" });
                        }


                    }
                }
            }
        }
    }

    // Display Gantt chart data for each core after execution
    for (int coreId = 0; coreId < ganttChartData.size(); ++coreId) {
        std::cout << "Gantt Chart for Core " << coreId << ":\n";
        displayGanttChart(ganttChartData[coreId]);
    }
}



queue<Process> Scheduler::getProcessQueue() {
    return processQueue;
}
