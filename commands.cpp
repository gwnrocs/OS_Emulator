#include "commands.h"
#include <thread>

void Commands::initialize() {
    Utils::printConfirmation("initialize");
}

void Commands::screen() {
    Utils::printConfirmation("screen");
}

void Commands::schedulerTest(std::shared_ptr<Scheduler>& scheduler) {
    int frequency = 10;
    std::atomic<bool> stopFlag(false);

    std::thread processGenerationThread(ProcessManager::startProcessGeneration, frequency, ref(scheduler), ref(stopFlag));
    processGenerationThread.detach();

    Utils::printConfirmation("Process generation started.");
}

void Commands::schedulerStop() {
    ProcessManager::stopProcessGeneration();    
}

void Commands::screenList(CPU& cpu) {
    ProcessManager::displayProcessList(cpu);  
}

void Commands::reportUtil(CPU& cpu) {
    ProcessManager::saveReportToFile(cpu, "csopesy-log.txt");
}