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

    SchedulingAlgorithm schedulingAlgorithm;
    std::vector<std::shared_ptr<Core>> coresAvailable;
    std::vector<std::shared_ptr<Screen>> readyQueue;
    std::shared_ptr<Memory> memory;

    Schedule();

    int initializeScheduler(const std::string& algo, std::shared_ptr<Memory> memory);
    void startScheduler();
    void debugSchedulerState();
    void executeRR();
    void executeFCFS();
    void initializeMemory(std::shared_ptr<Memory> memory);
};

#endif