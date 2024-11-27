#include "Schedule.h"
#include <iostream>

// Constructor
Schedule::Schedule() : schedulingAlgorithm(FCFS) {}

// Initialize scheduler
int Schedule::initialize_scheduler(const std::string& algoSelected, std::shared_ptr<Memory> memory) {
    std::string algo = algoSelected.substr(1, algoSelected.length() - 2);

    if (algo == "fcfs") {
        schedulingAlgorithm = FCFS;
        initialize_memory(memory);
        return 0;
    }
    else if (algo == "rr") {
        schedulingAlgorithm = RR;
        return 0;
    }
    else {
        throw std::invalid_argument("Invalid scheduling algorithm specified");
    }
}

// Initialize memory
void Schedule::initialize_memory(std::shared_ptr<Memory> memory) {
    this->memory = memory;
}

// Run scheduler
void Schedule::run_scheduler() {
    if (schedulingAlgorithm == FCFS) {
        run_fcfs();
    }
    else if (schedulingAlgorithm == RR) {
        run_rr();
    }
}

// FCFS scheduling algorithm
void Schedule::run_fcfs() {
    for (auto& core : coresAvailable) {
        if (!readyQueue.empty()) {
            if (core->process_to_execute == nullptr) {  // Core is free
                memory->allocate(readyQueue.front());
                core->process_to_execute = readyQueue.front();
                readyQueue.erase(readyQueue.begin());

                // Assign core ID and update status
                core->process_to_execute->core_id_assigned = core->id;
                core->process_to_execute->status = Screen::RUNNING;
            }
        }
        else {
            break;  // No processes left in the queue
        }
    }
}

// Round-Robin scheduling algorithm
void Schedule::run_rr() {
    for (auto& core : coresAvailable) {
        if (core->process_to_execute == nullptr) {
            if (!readyQueue.empty()) {
                memory->allocate(readyQueue.front());
                core->process_to_execute = readyQueue.front();
                readyQueue.erase(readyQueue.begin());

                core->process_to_execute->core_id_assigned = core->id;
                core->process_to_execute->status = Screen::RUNNING;
            }
        }
        else {
            // Preemption logic
            if (core->cycle == 0) {
                core->cycle = core->quantumCycle;
                readyQueue.push_back(core->process_to_execute);
                core->process_to_execute->status = Screen::READY;
                core->process_to_execute = nullptr;

                if (!readyQueue.empty()) {
                    memory->allocate(readyQueue.front());
                    core->process_to_execute = readyQueue.front();
                    readyQueue.erase(readyQueue.begin());

                    core->process_to_execute->core_id_assigned = core->id;
                    core->process_to_execute->status = Screen::RUNNING;
                }
            }
        }
    }
}

// Debug scheduler state
void Schedule::debugSchedulerState() {
    std::cout << "==== Scheduler State ====" << std::endl;

    std::cout << "Ready Queue:" << std::endl;
    for (const auto& screen : readyQueue) {
        std::cout << " - " << screen->processName << " (PID: " << screen->processId << ")" << std::endl;
    }

    std::cout << "Core States:" << std::endl;
    for (const auto& core : coresAvailable) {
        std::cout << "Core " << core->id << ": ";
        if (core->process_to_execute) {
            std::cout << "Running process " << core->process_to_execute->processName;
        }
        else {
            std::cout << "Free";
        }
        std::cout << std::endl;
    }

    std::cout << "=========================" << std::endl;
}
