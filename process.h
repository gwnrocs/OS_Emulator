#pragma once
#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "utils.h"
#include "runScreenLoop.h"
#include "ConfigManager.h"

using namespace std;

class Process {
public:
    Process();
    Process(string name, int inputID);

    enum ProcessStatus {
        Created,
        Waiting,
        Running,
        Done
    };

    void drawConsole();
    void printHelloWorld(int coreId, int timeToBurst);

    const string& getName() const;
    const int& getID() const;
    const string& getCreationTime() const;
    int getCurrentLine() const;
    int getTotalLines() const;
    int getCore() const;
    size_t getMemoryRequired() const;

    void updateCurrentLine(int line);
    void updateProcessStatus(ProcessStatus processStatus);
    ProcessStatus getStatus() const;
    void setTotalLines(int lines);
    void setName(const string& name);
    void setCreationTime(const string& time);
    void setCore(int core);

private:
    string processName;
    int id;
    int currentLine;
    int totalLines;
    string creationTime;
    int core;
    ProcessStatus status;
    size_t memoryRequired;
};

#endif