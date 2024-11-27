#include "Screen.h"
#include <iostream>

Screen::Screen(std::string processName, int processId, int currentLine, int totalLines,
    std::string creationTime, int memory_to_occupy, int frames_needed)
    : processName(processName), processId(processId), currentLine(currentLine),
    totalLines(totalLines), creationTime(creationTime),
    memory_to_occupy(memory_to_occupy), frames_needed(frames_needed),
    status(READY), core_id_assigned(-1), base_frame(-1), last_frame(-1),
    placed_in_memory(0) {}

void Screen::printScreen() {
    if (status != 3) {
        std::cout << "\nProcess Name: " << processName << "\n";
        std::cout << "Time Created: " << creationTime << "\n";

        std::string strStatus = "";
        switch (status) {
        case 0: strStatus = "Created"; break;
        case 1: strStatus = "Waiting"; break;
        case 2: strStatus = "Running"; break;
        case 3: strStatus = "Done";
        }

        std::cout << "Status: " << strStatus << "\n";
        std::cout << "ID: " << processId << "\n";
        std::cout << "Current Instruction Line: " << currentLine << "\n";
        std::cout << "Lines of Code: " << totalLines << "\n\n";
    }
    else {
        std::cout << "\nFinished!\n\n";
    }
}

int Screen::executeCommand() {
    currentLine++;
    if (currentLine == totalLines) {
        status = FINISHED;
        return 0;
    }
    return 1;
}