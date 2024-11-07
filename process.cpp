#include "process.h"
#include <iostream>
#include "ConfigManager.h"

using Config::configParams;
using namespace std;

Process::Process() : processName("Unnamed"), currentLine(0), creationTime(Utils::getCurrentTimestamp()) {
    status = Process::Created;
    core = -1;
    id = -1;
    totalLines = Utils::generateRandomNumber(configParams.min_ins, configParams.max_ins);
}

Process::Process(string name, int inputID) {
    processName = name;
    id = inputID;
    currentLine = 0;
    creationTime = Utils::getCurrentTimestamp();
    status = Process::Created;
    core = -1;
    totalLines = Utils::generateRandomNumber(configParams.min_ins, configParams.max_ins);
}

void Process::drawConsole() {
    auto showDetails = [&]() {
        if (status != 3) {
            cout << "\n  Process Name: " << processName << "\n";
            cout << "  Time Created: " << creationTime << "\n";

            // Convert Status enum to string
            string strStatus = "";
            switch (status) {
            case 0: strStatus = "Created"; break;
            case 1: strStatus = "Waiting"; break;
            case 2: strStatus = "Running"; break;
            case 3: strStatus = "Done";
            }

            cout << "  Status: " << strStatus  << "\n";

            //TODO: Change to actual process ID
            cout << "  ID: " << id << "\n";
            cout << "  Current Instruction Line: " << currentLine << "\n";
            cout << "  Lines of Code: " << totalLines << "\n\n";
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

    ifstream infile(fileName);
    bool fileExists = infile.good();

    ofstream outfile(fileName, fileExists ? ios_base::app : ios_base::out);

    if (!outfile) {
        cout << "Error opening file!" << endl;
        return;
    }

    if (!fileExists) {
        outfile << "Process name: " << processName << endl << endl;
        outfile << "Logs:" << endl;
        outfile << endl;
    }

    for (int i = 0; i < timeToBurst; i++) {
        if (currentLine < totalLines) {
            string timestamp = Utils::getCurrentTimestamp();
            outfile << "(" << timestamp << ") Core:" << coreId << " \"" << output << "\"" << endl;
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