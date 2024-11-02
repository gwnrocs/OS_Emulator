#include "cpu.h"
#include "utils.h"
#include "thread"
#include "chrono"

CPU::CPU(int num_cpu) {
    num_cores = num_cpu;
    coresStatus.resize(num_cpu, false);
    processInCore.resize(num_cpu);
}

void CPU::runCores() {
    int num_cpu, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec;
    string scheduler_type;

    // Load configuration values
    loadConfig(num_cpu, scheduler_type, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec);

    // Iterate over each core to execute the assigned process
    for (int i = 0; i < num_cores; i++) {
        if (coresStatus[i]) {  // Check if the core is busy
            if (delays_per_exec > 0) {
                // Introduce a delay before executing the instruction (busy-waiting)
                std::this_thread::sleep_for(std::chrono::milliseconds(delays_per_exec));
            }

            thread runCoreProcessThread([&]() {
                processInCore[i].printHelloWorld(i); // Execute the instruction for the process
                completeProcess(i); // Mark process as complete
                }); 

            runCoreProcessThread.detach(); // run parallel
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void CPU::completeProcess(int coreId) {
    coresStatus[coreId] = false;
    processInCore[coreId].updateProcessStatus(Process::Done);  
    processInCore[coreId] = Process();  
}

void CPU::removeProcess(int coreId) {
    coresStatus[coreId] = false;
    processInCore[coreId] = Process();  
}



void CPU::assignProcessToCore(Process& process, int coreId) {
    coresStatus[coreId] = true;
    processInCore[coreId] = process; 
}

vector<bool> CPU::getCoresStatus() const { return coresStatus; }
const int CPU::getNumCores() const { return num_cores; }
