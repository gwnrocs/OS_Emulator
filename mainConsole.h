#pragma once
#ifndef MAINCONSOLE_H
#define MAINCONSOLE_H

#include "abstractConsole.h"
#include "baseScreen.h"
#include "cpu.h"
#include "scheduler.h"
#include "utils.h"
#include "cpu.h"
#include "scheduler.h"
#include "process_manager.h"
#include "commands.h"

class MainConsole : public AConsole 
{
public:
    MainConsole();
    ~MainConsole(); // Destructor to delete the dynamically allocated CPU object
    void printHeading();
    void onEnabled() override;
    void display() override;
    void process() override;

private:
    bool running;
    bool initialized;
    CPU* cpu;  // Use a pointer to CPU instead of a direct object
    Scheduler scheduler;
    void processCommand(std::string);
};

#endif