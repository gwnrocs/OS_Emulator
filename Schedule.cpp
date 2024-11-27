#include "Schedule.h"
#include <iostream>\

using namespace std;

// Constructor
Schedule::Schedule() : schedulingAlgorithm(FCFS) {}

// Initialize scheduler
int Schedule::initializeScheduler(const std::string& algoSelected, std::shared_ptr<Memory> memory) {
    std::string algo = algoSelected.substr(1, algoSelected.length() - 2);

    if (algo == "fcfs") {
        schedulingAlgorithm = FCFS;
        initializeMemory(memory);
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
void Schedule::initializeMemory(std::shared_ptr<Memory> memory) {
    this->memory = memory;
}

// Run scheduler
void Schedule::startScheduler() {
    if (schedulingAlgorithm == FCFS) {
        executeFCFS();
    }
    else if (schedulingAlgorithm == RR) {
        executeRR();
    }
}

// FCFS scheduling algorithm
void Schedule::executeFCFS() {
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
void Schedule::executeRR() {
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
    cout << "==== Scheduler State ====" << endl;

    cout << "Ready Queue:" << endl;
    for (const auto& screen : readyQueue) {
        cout << " - " << screen->processName << " (PID: " << screen->processId << ")" << endl;
    }

    cout << "Core States:" << endl;
    for (const auto& core : coresAvailable) {
        cout << "Core " << core->id << ": ";
        if (core->process_to_execute) {
            cout << "Running process " << core->process_to_execute->processName;
        }
        else {
            cout << "Free";
        }
        cout << endl;
    }

    cout << "=========================" << endl;
}