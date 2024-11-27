#ifndef MainConsole_H
#define MainConsole_H

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

class MainConsole {
private:
    Schedule scheduler;
    std::shared_ptr<Memory> memory;
    std::vector<std::shared_ptr<Screen>> screens;

    bool initialized = false;
    bool isRunning = false;
    bool allowProcessCreation = false;
    bool isMainMenu = true;
    bool showConfig = false;

    int nCpuToInitialize = -1;
    std::string schedulerType = "";
    int quantumCycles = -1;
    int maxMemory = -1;
    int delayExecFake = -1;
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
    bool checkExistingScreen(const std::string& 
    
    );
    bool isIdleCycle();
    void initializeCores(int numCores, int delay, int quantumCycles, std::shared_ptr<Memory> memory);
    void attachScreen(const std::string& processName);
    void createScreen(const std::string& processName);
    void initScreen(std::shared_ptr<Screen> screen);
    void handleScreenCommand(const std::string& option, const std::string& processName);
    void listScreens(bool debug = false);
    void joinAllThreads();
    void printConfig();
    void generateReportUtil();
    void generateProcessSMI();

public:
    MainConsole();
    void drawConsole();
    void initialize();
    void schedulerTest();
    void simulateCpuCycle();
};

#endif