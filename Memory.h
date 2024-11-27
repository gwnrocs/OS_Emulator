#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <unordered_map>
#include <memory>
#include "Screen.h"

class Memory {
public:
    std::unordered_map<int, bool> flatMemoryAllocator;    // Used in flat memory
    std::vector<int> availableFrames;                     // Used in paging
    std::vector<std::shared_ptr<Screen>> processInMemory;
    int numberOfTotalFrames = -1;
    int maxMemory = 0;
    int type = 0;

    int numPagedIn = 0;
    int numPagedOut = 0;

    // Constructor
    Memory();

    // Public methods
    void initialize(int max, int memoryPerFrame, int type);
    void allocate(std::shared_ptr<Screen> screen);
    void deallocate(std::shared_ptr<Screen> screen);
    int getMemoryUsage();

private:
    // Private utility methods
    bool checkIfProcessExistsInMemory(std::shared_ptr<Screen> screen);
    void allocateFlatMemory(std::shared_ptr<Screen> screen);
    void deallocateFlatMemory(std::shared_ptr<Screen> screen);
    void deallocateFrames(std::shared_ptr<Screen> screen);
    void deleteProcessInMemory(std::shared_ptr<Screen> screen);
    int returnFirstFitIndex(int framesNeeded);
    void storeBackingStore();

    void allocateFrames(std::shared_ptr<Screen> screen);
    bool takeBackingStore(std::shared_ptr<Screen> screen);
};

#endif
