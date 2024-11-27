#include "Screen.h"
#include <iostream>

Screen::Screen(std::string process_name, int pid, int curr_line_instr, int total_line_instr,
    std::string created_at, int memory_to_occupy, int frames_needed)
    : process_name(process_name), pid(pid), curr_line_instr(curr_line_instr),
    total_line_instr(total_line_instr), created_at(created_at),
    memory_to_occupy(memory_to_occupy), frames_needed(frames_needed),
    status(READY), core_id_assigned(-1), base_frame(-1), last_frame(-1),
    placed_in_memory(0) {}

void Screen::printScreen() {
    if (status != 3) {
        std::cout << "\nProcess Name: " << process_name << "\n";
        std::cout << "Time Created: " << created_at << "\n";

        std::string strStatus = "";
        switch (status) {
        case 0: strStatus = "Created"; break;
        case 1: strStatus = "Waiting"; break;
        case 2: strStatus = "Running"; break;
        case 3: strStatus = "Done";
        }

        std::cout << "Status: " << strStatus << "\n";
        std::cout << "ID: " << pid << "\n";
        std::cout << "Current Instruction Line: " << curr_line_instr << "\n";
        std::cout << "Lines of Code: " << total_line_instr << "\n\n";
    }
    else {
        std::cout << "\nFinished!\n\n";
    }
}

int Screen::executeCommand() {
    curr_line_instr++;
    if (curr_line_instr == total_line_instr) {
        status = FINISHED;
        return 0;
    }
    return 1;
}