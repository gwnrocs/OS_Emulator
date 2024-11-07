#include "commands.h"
#include <thread>
#include "ConfigManager.h"
using Config::ConfigParams;


void Commands::initialize() {
    Utils::printConfirmation("initialize");
}

void Commands::screen() {
    Utils::printConfirmation("screen");
}

void Commands::schedulerTest(std::shared_ptr<Scheduler>& scheduler) {
    int frequency = configParams.batch_freq;
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