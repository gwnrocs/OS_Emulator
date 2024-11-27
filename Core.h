#ifndef CORE_H
#define CORE_H

#include <memory>
#include "Screen.h"
#include "Memory.h"

class Core {
public:
    int id;
    int delayConfig;
    int delay;
    int cycle;
    int quantumCycle;
    std::shared_ptr<Screen> process_to_execute;
    std::shared_ptr<Memory> memory;

    Core(int id, int delayConfig, int quantumCycle, std::shared_ptr<Memory> memory);

    void run_core();
};

#endif
