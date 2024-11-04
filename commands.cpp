#include "commands.h"
#include <thread>

void Commands::initialize() {
    Utils::printConfirmation("initialize");
}

void Commands::screen() {
    Utils::printConfirmation("screen");
}

void Commands::schedulerTest(std::shared_ptr<Scheduler>& scheduler, int batchFreq) {
    int frequency = batchFreq;
    std::atomic<bool> stopFlag(false);

    std::thread processGenerationThread(ProcessManager::startProcessGeneration, frequency, ref(scheduler), ref(stopFlag));
    processGenerationThread.detach();

    cout << Colors::Green << "Generating processes . . . \n" << Colors::White;
}

void Commands::schedulerStop() {
    ProcessManager::stopProcessGeneration();    
    cout << Colors::Red << "Stopped generating processes . . . \n" << Colors::White;
}

void Commands::screenList(CPU& cpu) {
    ProcessManager::displayProcessList(cpu);  
}

void Commands::reportUtil(CPU& cpu) {
    ProcessManager::saveReportToFile(cpu, "csopesy-log.txt");
}