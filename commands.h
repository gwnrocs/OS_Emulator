#pragma once
#ifndef COMMANDS_H
#define COMMANDS_H

#include "process_manager.h"
#include "cpu.h"
#include "scheduler.h"
#include "utils.h"

namespace Commands {
    void initialize();
    void screen();
    void schedulerTest(CPU& cpu, Scheduler& scheduler);
    void schedulerStop();
    void screenList();
    void reportUtil();
}

#endif

