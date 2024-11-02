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
    Scheduler scheduler;
};

#endif
