#include "commands.h"
#include <thread>

void Commands::initialize() {
    Utils::printConfirmation("initialize");
}

void Commands::screen() {
    Utils::printConfirmation("screen");
}

void Commands::schedulerTest(CPU& cpu, Scheduler& scheduler) {
    int frequency = 10;
    atomic<bool> stopFlag(false);

    thread processThread(manageProcesses, ref(cpu), ref(scheduler), ref(stopFlag), frequency);
    processThread.detach();
}

void Commands::schedulerStop() {
    ProcessManager::stopProcessGeneration();
}

void Commands::screenList() {
    int num_cpu, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec;
    string scheduler_type;

    // Load the configuration to get the number of CPU cores
    loadConfig(num_cpu, scheduler_type, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec);

    CPU cpuInstance(num_cpu);  // Initialize the CPU with the number of cores from config.txt
    ProcessManager::displayProcessList(cpuInstance);  // Pass the CPU instance to the function
}


void Commands::reportUtil() {
    Utils::printConfirmation("report-util");
}
