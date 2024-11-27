#include "Core.h"

Core::Core(int id, int delayConfig, int quantumCycle, std::shared_ptr<Memory> memory)
    : id(id), delayConfig(delayConfig), delay(delayConfig), cycle(quantumCycle),
    quantumCycle(quantumCycle), memory(memory), process_to_execute(nullptr) {}

void Core::run_core() {
    if (process_to_execute != nullptr) {
        if (delay == 0) {
            delay = delayConfig;
            if (process_to_execute->executeCommand() == 0) {
                memory->deallocate(process_to_execute);
                process_to_execute = nullptr;
                cycle = quantumCycle;
            }
            else {
                cycle--;
            }
        }
        else {
            delay--;
        }
    }
}