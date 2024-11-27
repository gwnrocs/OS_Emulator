#pragma once
#ifndef SCREEN_H
#define SCREEN_H

#include <string>
#include <vector>
#include <ctime>

class Screen {
public:
    enum ProcessStatus {
        READY,
        RUNNING,
        WAITING,
        FINISHED
    };

    std::string processName;
    ProcessStatus status;
    int processId;
    int currentLine;
    int totalLines;
    int core_id_assigned;
    std::string creationTime;
    int frames_needed;
    int base_frame;
    int last_frame;
    int memory_to_occupy;
    time_t placed_in_memory;
    std::vector<int> pages;

    Screen(std::string processName, int pid, int currentLine, int totalLines,
        std::string creationTime, int memory_to_occupy, int frames_needed);

    void printScreen();
    int executeCommand();
};

#endif