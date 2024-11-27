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

    std::string process_name;
    ProcessStatus status;
    int pid;
    int curr_line_instr;
    int total_line_instr;
    int core_id_assigned;
    std::string created_at;
    int frames_needed;
    int base_frame;
    int last_frame;
    int memory_to_occupy;
    time_t placed_in_memory;
    std::vector<int> pages;

    Screen(std::string process_name, int pid, int curr_line_instr, int total_line_instr,
        std::string created_at, int memory_to_occupy, int frames_needed);

    void printScreen();
    int executeCommand();
};

#endif