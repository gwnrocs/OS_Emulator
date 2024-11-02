#include "scheduler.h"
#include <fstream>
#include <iostream>
#include <string>
#include <queue>

using namespace std;


struct ExecutionRecord {
    std::string processName;
    int coreIndex;
    int startTime;
    int endTime;
};


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
    }
    else if (scheduler_type == "rr") {
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

void displayGanttChart(const std::vector<ExecutionRecord>& data) {
    for (const auto& record : data) {
        std::cout << "Core " << record.coreIndex << ": [" << record.startTime << "-" << record.endTime
                  << "] Process: " << record.processName << std::endl;
    }
}

void Scheduler::printProcessQueue() {
    std::queue<Process> tmp_q = processQueue;  // Create a copy of the queue

    if (tmp_q.empty()) {
        std::cout << "Process queue is empty." << std::endl;
        return;
    }

    std::cout << "Current Process Queue:" << std::endl;
    while (!tmp_q.empty()) {
        Process q_element = tmp_q.front();
        std::cout << "Process Name: " << q_element.getName()
                  << " | Total Lines: " << q_element.getTotalLines()
                  << " | Address: " << &q_element
                  << std::endl;
        tmp_q.pop();
    }
}

void Scheduler::RoundRobin(CPU& cpu, int quantum_cycles) {
    int currentTime = 0;  
    std::vector<ExecutionRecord> ganttChartData;

    printProcessQueue();

    while (!processQueue.empty()) {
        Process current_process = processQueue.front();
        int core_index = -1;

         for (int i = 0; i < cpu.getNumCores(); ++i) {
            if (!cpu.getCoresStatus()[i]) {
                core_index = i;
                break;
            }
        }

        if (core_index != -1) {
            processQueue.pop();
            cpu.assignProcessToCore(current_process, core_index);

            int startTime = currentTime;
            for (int i = 0; i < quantum_cycles; ++i) {
                ++currentTime;
                current_process.executeInstruction();
                if (current_process.getStatus() == Process::Done) {
                    break;
                }
            }

            cpu.removeProcess(core_index);

            ganttChartData.push_back({current_process.getName(), core_index, startTime, currentTime});
            if (current_process.getStatus() != Process::Done) {
                processQueue.push(current_process);
            } 

        } else {
            break;
        }


    }

    displayGanttChart(ganttChartData);
}


queue<Process> Scheduler::getProcessQueue() {
    return processQueue;
}
