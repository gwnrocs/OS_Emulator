#include "Memory.h"
#include <stdexcept>
#include <optional>
#include <fstream>
#include <iostream>
using namespace std;

Memory::Memory() {}

void Memory::initialize(int max, int memoryPerFrame, int type)
{
    maxMemory = max;
    numberOfTotalFrames = maxMemory / memoryPerFrame;
    this->type = type;

    if (type == 0)
        for (int i = 0; i < numberOfTotalFrames; i++)
            flatMemoryAllocator.insert({ i, false });
    else
        for (int i = 0; i < numberOfTotalFrames; i++)
            availableFrames.push_back(i);
}


void Memory::deallocateFrames(std::shared_ptr<Screen> screen)
{
    for (int i = 0; i < screen->frames_needed; i++)
        availableFrames.push_back(screen->pages[i]);
    screen->pages.clear();
    deleteProcessInMemory(screen);
}


void Memory::deleteProcessInMemory(std::shared_ptr<Screen> screen)
{
    for (int i = 0; i < processInMemory.size(); i++)
    {
        if (processInMemory[i] == screen) {
            processInMemory.erase(processInMemory.begin() + i);
            break;
        }
    }
}


int Memory::returnFirstFitIndex(int framesNeeded)
{
    int endRange = -1;
    int increment = 0;
    for (int i = 0; i < numberOfTotalFrames; i++)
    {
        if (availableFrames[i] == false) {
            endRange = i;
            increment++;
        }
        else {
            endRange = -1;
            increment = 0;
        }

        if (increment == framesNeeded && endRange != -1)
            return endRange - (framesNeeded - 1); 
    }

    return -1;
}


void Memory::allocate(std::shared_ptr<Screen> screen)
{
    if (!checkIfProcessExistsInMemory(screen)) {
        if (type == 0)
            allocateFlatMemory(screen);
        else
            allocateFrames(screen);
    }
}


void Memory::storeBackingStore() 
{
    if (processInMemory.size() != 0) {
        int index = 0; 
        for (int i = 0; i < processInMemory.size(); i++)
        {
            if (processInMemory[index]->placed_in_memory > processInMemory[i]->placed_in_memory) // index is more recent than i
                index = i;
        }

        ofstream fileOPESY;
        fileOPESY.open(processInMemory[index]->processName + ".txt");
        fileOPESY.close();

        deallocate(processInMemory[index]);
        numPagedOut++;
    }
}


bool Memory::takeBackingStore(std::shared_ptr<Screen> screen)
{
    try {
        // take
        std::string fileToRemove = screen->processName + ".txt";
        std::remove(fileToRemove.c_str());

        numPagedIn++;
        return true;
    }
    catch (exception e) {
        return false;
    }
}

void Memory::deallocate(std::shared_ptr<Screen> screen)
{
    try {
        if (checkIfProcessExistsInMemory(screen)) {
            if (type == 0)
                deallocateFlatMemory(screen);
            else
                deallocateFrames(screen);
        }
        else
            throw;
    }
    catch (exception e) {
        std::cout << "Tried to deallocate a process that does not exist: " << e.what() << std::endl;
    }
}


void Memory::allocateFlatMemory(std::shared_ptr<Screen> screen)
{
    takeBackingStore(screen);

    bool isProcessAllocated = false;
    while (!isProcessAllocated)
    {

        int firstFitIndex = returnFirstFitIndex(screen->frames_needed);

        if (firstFitIndex != -1)
        {
            screen->base_frame = firstFitIndex;
            screen->last_frame = firstFitIndex + (screen->frames_needed - 1);

            for (int i = firstFitIndex; i <= firstFitIndex + (screen->frames_needed - 1); i++)
                flatMemoryAllocator[i] = true;

            processInMemory.push_back(screen);
            screen->placed_in_memory = std::time(nullptr);
            isProcessAllocated = true;

        }
        else 
            storeBackingStore();
    }
}


void Memory::deallocateFlatMemory(std::shared_ptr<Screen> screen)
{
    for (int i = screen->base_frame; i <= screen->last_frame; i++)
        flatMemoryAllocator[i] = false;

    screen->base_frame = -1;
    screen->last_frame = -1;

    deleteProcessInMemory(screen);
}


void Memory::allocateFrames(std::shared_ptr<Screen> screen)
{
    takeBackingStore(screen);

    bool isProcessAllocated = false;
    while (!isProcessAllocated)
    {
        if (availableFrames.size() >= screen->frames_needed)
        {
            for (int i = 0; i < screen->frames_needed; i++)
                screen->pages.push_back(availableFrames[i]);

            availableFrames.erase(availableFrames.begin() + 0, availableFrames.begin() + screen->frames_needed); // recheck on this
            processInMemory.push_back(screen);
            screen->placed_in_memory = std::time(nullptr);
            isProcessAllocated = true;
        }
        else 
            storeBackingStore();
    }

}


bool Memory::checkIfProcessExistsInMemory(std::shared_ptr<Screen> screen)
{
    for (int i = 0; i < processInMemory.size(); i++)
    {
        if (processInMemory[i] == screen)
            return true;
    }
    return false;
}


int Memory::getMemoryUsage()
{
    int total = 0;
    for (int i = 0; i < processInMemory.size(); i++)
    {
        total += processInMemory[i]->memory_to_occupy;
    }

    return total;
}