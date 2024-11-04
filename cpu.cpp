#include "cpu.h"
#include "utils.h"
#include "thread"
#include "chrono"
#include "process_manager.h"

CPU::CPU(int num_cpu) {
    num_cores = num_cpu;
    coresStatus.resize(num_cpu, false);
    processInCore.resize(num_cpu);
    usedCores = 0;
}

void CPU::runCores() {
    int num_cpu, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec;
    string scheduler_type;

    loadConfig(num_cpu, scheduler_type, quantum_cycles, min_ins, max_ins, batch_freq, delays_per_exec);

    for (int i = 0; i < num_cores; i++) {
        if (coresStatus[i]) {  // Check if the core is busy
            if (delays_per_exec > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(delays_per_exec));
            }

            thread runCoreProcessThread([&]() {

                if(scheduler_type == "fcfs") {
                    processInCore[i].printHelloWorld(i , processInCore[i].getTotalLines());
                    completeProcess(i);
                } else {
                    processInCore[i].printHelloWorld(i, 1);
                }
            }); 

            runCoreProcessThread.detach(); // run parallel
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void CPU::completeProcess(int coreId) {
    coresStatus[coreId] = false;
    processInCore[coreId].updateProcessStatus(Process::Done);  
    usedCores--;
    processInCore[coreId] = Process();  
}

void CPU::removeProcess(int coreId) {
    coresStatus[coreId] = false;
    usedCores--;
    Process* process = ProcessManager::findProcess(processInCore[coreId].getName());
    process->updateProcessStatus(Process::Waiting);
    processInCore[coreId] = Process();  
    processInCore[coreId].setCore(-1);
}

void CPU::assignProcessToCore(Process& process, int coreId) {
    coresStatus[coreId] = true;
    processInCore[coreId] = process; 
    process.updateProcessStatus(Process::Running); 
    usedCores++;
    ProcessManager::updateProcess(process, coreId); // screen -ls temp fix
}

bool CPU::areAllCoresIdle() const {
    for (const auto& status : coresStatus) {
        if (status) { 
            return false;
        }
    }
    return true; // All cores are idle
}

Process& CPU::getProcessOnCore(int coreId) {
    if (coreId < 0 || coreId >= processInCore.size() || !coresStatus[coreId]) {
        throw std::out_of_range("Core ID is invalid or core is idle");
    }
    return processInCore[coreId];
}

vector<bool> CPU::getCoresStatus() const { return coresStatus; }
const int CPU::getNumCores() const { return num_cores; }
int CPU::getUsedCores() const { return usedCores; }