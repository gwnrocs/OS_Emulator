#include "process.h"
#include <iostream>

using namespace std;

Process::Process() : processName("Unnamed"), currentLine(0), creationTime(Utils::getCurrentTimestamp()) {
    int num_cpu, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec, id;
    string scheduler;
    status = Process::Created;
    core = -1;
    id = -1;

    loadConfig(num_cpu, scheduler, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec);

    totalLines = Utils::generateRandomNumber(min_ins, max_ins);
}

Process::Process(string name, int inputID) {
    processName = name;
    id = inputID;
    currentLine = 0;
    creationTime = Utils::getCurrentTimestamp();
    status = Process::Created;
    core = -1;

    int num_cpu, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec;
    string scheduler;

    // Load configuration to set instruction limits
    loadConfig(num_cpu, scheduler, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec);

    // Set total instructions between the minimum and maximum instruction limits
    totalLines = Utils::generateRandomNumber(min_ins, max_ins);
}

void Process::drawConsole() {
    auto showDetails = [&]() {
        if (status != 3) {
            std::cout << "\n  Process Name: " << processName << "\n";
            std::cout << "  Time Created: " << creationTime << "\n";

            // Convert Status enum to string
            std::string strStatus = "";
            switch (status) {
            case 0: strStatus = "Created"; break;
            case 1: strStatus = "Waiting"; break;
            case 2: strStatus = "Running"; break;
            case 3: strStatus = "Done";
            }

            cout << "  Status: " << strStatus  << "\n";

            //TODO: Change to actual process ID
            std::cout << "  ID: " << id << "\n";
            std::cout << "  Current Instruction Line: " << currentLine << "\n";
            std::cout << "  Lines of Code: " << totalLines << "\n\n";
        }
        else {
            cout << "Finished!";
        }
        };

    runScreenLoop(showDetails);
}

void Process::printHelloWorld(int coreId, int timeToBurst) {
    string output = "Hello world from " + processName + "!";
    string fileName = "Logs/" + processName + ".txt";

    std::ifstream infile(fileName);
    bool fileExists = infile.good();

    std::ofstream outfile(fileName, fileExists ? std::ios_base::app : std::ios_base::out);

    if (!outfile) {
        std::cout << "Error opening file!" << std::endl;
        return;
    }

    if (!fileExists) {
        outfile << "Process name: " << processName << std::endl << std::endl;
        outfile << "Logs:" << std::endl;
        outfile << std::endl;
    }

    for (int i = 0; i < timeToBurst; i++) {
        if (currentLine < totalLines) {
            std::string timestamp = Utils::getCurrentTimestamp();
            outfile << "(" << timestamp << ") Core:" << coreId << " \"" << output << "\"" << std::endl;
            currentLine += 1;
        }
        
        if (currentLine == totalLines) {
            updateProcessStatus(Process::Done);
        }
    }

    outfile.close();
}


const string& Process::getName() const { return processName; }
const int& Process::getID() const { return id; }
const string& Process::getCreationTime() const { return creationTime; }
int Process::getCurrentLine() const { return currentLine; }
int Process::getTotalLines() const { return totalLines; }
int Process::getCore() const { return core; }

void Process::updateCurrentLine(int line) { currentLine = line; }
void Process::updateProcessStatus(ProcessStatus processStatus) { status = processStatus; }
Process::ProcessStatus Process::getStatus() const { return status; }
void Process::setTotalLines(int lines) { totalLines = lines; }
void Process::setName(const string& name) { processName = name; }
void Process::setCreationTime(const string& time) { creationTime = time; }
void Process::setCore(int newCore) { core = newCore; }