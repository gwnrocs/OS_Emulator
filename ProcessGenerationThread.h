// ProcessGenerationThread.h

#ifndef PROCESS_GENERATION_THREAD_H
#define PROCESS_GENERATION_THREAD_H

#include <thread>
#include <atomic>
#include <vector>
#include <memory>
#include "process.h"
#include "scheduler.h"

class ProcessGenerationThread : public enable_shared_from_this<ProcessGenerationThread> {
    private:
        thread generationThread;
        atomic<bool> running;
        vector<shared_ptr<Process>> processQueue;

        void generateProcesses(std::shared_ptr<Scheduler>& scheduler);

    public:
        ProcessGenerationThread();

        void start(std::shared_ptr<Scheduler>& scheduler);
        void stop();
        ~ProcessGenerationThread();

        bool isRunning();
};

#endif 
