#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <vector>
#include <memory>
#include "Core.h"
#include "Screen.h"
#include "Memory.h"

class Schedule {
public:
    enum SchedulingAlgorithm {
        FCFS,
        RR
    };

    SchedulingAlgorithm schedulingAlgo;
    std::vector<std::shared_ptr<Core>> coresAvailable;
    std::vector<std::shared_ptr<Screen>> readyQueue;
    std::shared_ptr<Memory> memory;

    Schedule();

    int initialize_scheduler(const std::string& algo, std::shared_ptr<Memory> memory);
    void run_scheduler();
    void debugSchedulerState();
    void run_rr();
    void run_fcfs();
    void initialize_memory(std::shared_ptr<Memory> memory);
};

#endif
