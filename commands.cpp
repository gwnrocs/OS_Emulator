#include "commands.h"
#include <thread>
#include <memory>
#include <atomic>
#include "ConfigManager.h"
#include "ProcessGenerationThread.h"
#include "process_manager.h"
#include "Utils.h"

using Config::ConfigParams;

std::shared_ptr<ProcessGenerationThread> processGenerator;  // Shared instance for the process generator

void Commands::initialize() {
    Utils::printConfirmation("initialize");
}

void Commands::screen() {
    Utils::printConfirmation("screen");
}

void Commands::schedulerTest(std::shared_ptr<Scheduler>& scheduler) {


    if (processGenerator && processGenerator->isRunning()) {
        processGenerator->stop();
    }

    processGenerator = std::make_shared<ProcessGenerationThread>();
    processGenerator->start(scheduler);

    Utils::printConfirmation("Process generation started.");
}

void Commands::schedulerStop() {
    if (processGenerator && processGenerator->isRunning()) {
        processGenerator->stop();
        Utils::printConfirmation("Process generation stopped.");
    } else {
        Utils::printConfirmation("No process generation to stop.");
    }
}

void Commands::screenList(CPU& cpu) {
    ProcessManager::displayProcessList(cpu);
}

void Commands::reportUtil(CPU& cpu) {
    ProcessManager::saveReportToFile(cpu, "csopesy-log.txt");
}
