#ifndef CONSOLE_H
#define CONSOLE_H

#include <vector>
#include <string>
#include <thread>
#include <memory>
#include <iostream>
#include <string>
#include <windows.h>
#include "Core.h"
#include "Schedule.h"
#include "Screen.h"
#include "Memory.h"
#include "Utils.h"

class Console {
private:
    Schedule scheduler;
    std::shared_ptr<Memory> memory;
    std::vector<std::shared_ptr<Screen>> screens;

    bool hasInitialized = false;
    bool hasQuit = false;
    bool toStartCreatingProcess = false;
    bool isMainMenu = true;

    int freqProcess = -1;
    int freq = 0;
    int minCommand = -1;
    int maxCommand = -1;
    int delayExec = -1;
    int currentProcess = 0;
    int memoryPerFrame = 0;
    int minMemPerProc = 0;
    int maxMemPerProc = 0;

    int cpuCycles = 0;
    int idleCycles = 0;
    int activeCycles = 0;

    std::thread cpuCycleThreadHolder;
    std::vector<std::thread> listOfCoreThreads;

    // Private utility functions
    int checkCoresUsed();
    bool checkExistingScreen(const std::string& process_name);
    bool isIdleCycle();
    void initializeCores(int numCores, int delay, int quantumCycles, std::shared_ptr<Memory> memory);
    void attachScreen(const std::string& process_name);
    void createScreen(const std::string& process_name);
    void initScreen(std::shared_ptr<Screen> screen);
    void handleScreenCommand(const std::string& option, const std::string& process_name);
    void listScreens(bool debug = false);
    void joinAllThreads();

public:
    Console();
    void start();
    void initialize();
    void scheduler_test();
    void simulateCpuCycle();
};

#endif
