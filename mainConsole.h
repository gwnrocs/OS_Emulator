#pragma once
#ifndef MAINCONSOLE_H
#define MAINCONSOLE_H

#include "cpu.h"
#include "scheduler.h"
#include "utils.h"
#include "cpu.h"
#include "scheduler.h"
#include "process_manager.h"
#include "commands.h"

class MainConsole {
public:
    MainConsole();
    ~MainConsole(); // Destructor to delete the dynamically allocated CPU object
    void printHeading();
    void drawConsole();

private:
    bool running;
    bool initialized;
    CPU* cpu;  // Use a pointer to CPU instead of a direct object
    std::shared_ptr<Scheduler> scheduler;
    int num_cpu, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec;
    std::string scheduler_type;

};

#endif