#include "process.h"
#include <iostream>

using namespace std;

Process::Process() : processName("Unnamed"), currentLine(0), creationTime(Utils::getCurrentTimestamp()) {
    int num_cpu, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec;
    string scheduler;
    status = Process::Created;

    loadConfig(num_cpu, scheduler, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec);

    totalLines = Utils::generateRandomNumber(min_ins, max_ins);
}

Process::Process(string name) {
    processName = name;
    currentLine = 0;
    creationTime = Utils::getCurrentTimestamp();
    status = Process::Created;

    int num_cpu, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec;
    string scheduler;

    // Load configuration to set instruction limits
    loadConfig(num_cpu, scheduler, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec);

    // Set total instructions between the minimum and maximum instruction limits
    totalLines = Utils::generateRandomNumber(min_ins, max_ins);
}

void Process::displayInfo() const {
    cout << "\n  =============\n"
        << "  Process: " << processName << '\n'
        << "  Current Instruction: " << currentLine << '/' << totalLines << '\n'
        << "  Created At: " << creationTime << '\n'
        << "  =============\n\n";
}

void Process::drawConsole() {
    auto showDetails = [&]() {
        displayInfo();
     };
    runScreenLoop(showDetails);
}

void Process::printHelloWorld(int coreId) {
    string output = "Hello world from " + processName + "!";
    string fileName = processName + ".txt";
    ofstream outfile(fileName);

    if (!outfile) {
        cout << "Error opening file!" << endl;
        return;
    }

    outfile << "Process name: " << processName << endl << endl;
    outfile << "Logs:" << endl;
    outfile << endl;

    for (int i = 0; i < totalLines; i++)
    {
        string timestamp = Utils::getCurrentTimestamp();
        outfile << "(" << timestamp << ") Core:" << coreId << " \"" << output << "\"" << endl;
        currentLine += 1;
    }

    outfile.close();
}

const string& Process::getName() const { return processName; }
const string& Process::getCreationTime() const { return creationTime; }
int Process::getCurrentLine() const { return currentLine; }
int Process::getTotalLines() const { return totalLines; }
void Process::updateCurrentLine(int line) { currentLine = line; }
void Process::updateProcessStatus(ProcessStatus processStatus) { status = processStatus; }
Process::ProcessStatus Process::getStatus() const { return status; }

// temporary
void Process::executeInstruction() {
    if (currentLine < totalLines) {
        currentLine++;
    }

    if (currentLine == totalLines) {
        updateProcessStatus(Process::Done);
    } 
  
}
