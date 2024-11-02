#pragma once
#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "utils.h"
#include "runScreenLoop.h"

using namespace std;

class Process {
public:
    Process();
    Process(string name);

    enum ProcessStatus {
        Created,
        Waiting,
        Running,
        Done
    };

    void drawConsole();
    void printHelloWorld(int coreId);

    const string& getName() const;
    const string& getCreationTime() const;
    int getCurrentLine() const;
    int getTotalLines() const;
    void updateCurrentLine(int line);
    void updateProcessStatus(ProcessStatus processStatus);
    void executeInstruction();
    ProcessStatus getStatus() const;
    void displayInfo() const;

private:
    string processName;
    int currentLine;
    int totalLines;
    string creationTime;
    ProcessStatus status;
};

#endif