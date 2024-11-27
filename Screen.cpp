#include "Screen.h"
#include <iostream>

using namespace std;

Screen::Screen(std::string processName, int processId, int currentLine, int totalLines,
    std::string creationTime, int memory_to_occupy, int frames_needed)
    : processName(processName), processId(processId), currentLine(currentLine),
    totalLines(totalLines), creationTime(creationTime),
    memory_to_occupy(memory_to_occupy), frames_needed(frames_needed),
    status(READY), core_id_assigned(-1), base_frame(-1), last_frame(-1),
    placed_in_memory(0) {}

void Screen::printScreen() {
    if (status != 3) {
        cout << "\nProcess Name: " << processName  << endl
             << "Time Created: "   << creationTime << endl;

        std::string strStatus = "";
            switch (status) {
            case 0: strStatus = "Created"; break;
            case 1: strStatus = "Waiting"; break;
            case 2: strStatus = "Running"; break;
            case 3: strStatus = "Done";
        }

        cout << "Status: "                   << strStatus   << "\n"
             << "ID: "                       << processId   << "\n"
             << "Current Instruction Line: " << currentLine << "\n"
             << "Lines of Code: "            << totalLines  << "\n\n";
    }
    else {
        cout << "\nFinished!\n\n";
    }
}

int Screen::executeCommand() {
    currentLine++;
    if (currentLine == totalLines) {
        status = DONE;
        return 0;
    }
    return 1;
}