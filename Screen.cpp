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
    std::cout << "Process Name: " << process_name << "\nPID: " << pid
        << "\nCurrent Instruction Line: " << curr_line_instr
        << "\nLines of Code: " << total_line_instr
        << "\nCreated At: " << created_at << std::endl;

    if (status == FINISHED)
        std::cout << "Finished!" << std::endl;
}

int Screen::executeCommand() {
    curr_line_instr++;
    if (curr_line_instr == total_line_instr) {
        status = FINISHED;
        return 0;
    }
    return 1;
}