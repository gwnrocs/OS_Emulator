#include "ProcessGenerationThread.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "scheduler.h"
#include "process_manager.h"
#include "ConfigManager.h"

using Config::ConfigParams;
ProcessGenerationThread::ProcessGenerationThread() : running(false) {}

void ProcessGenerationThread::generateProcesses(shared_ptr<Scheduler>& scheduler) {
    while (running.load()) {
        ProcessManager::createScheduledProcess(scheduler);

        this_thread::sleep_for(chrono::milliseconds(configParams.batch_freq * 100));

        if (!running.load()) {
            cout << "BREAKING" << endl;
            break;
        }
    }
}


void ProcessGenerationThread::start(shared_ptr<Scheduler>& scheduler) {
    if (!running.load()) {
        running.store(true);

        shared_ptr<ProcessGenerationThread> self = shared_from_this();
        generationThread = thread([self, &scheduler]() {
            self->generateProcesses(scheduler);
        });
        generationThread.detach();
    }
}


bool ProcessGenerationThread::isRunning() {
    return running.load();
}

void ProcessGenerationThread::stop() {
    running.store(false);
}

ProcessGenerationThread::~ProcessGenerationThread() {
    stop();
}
