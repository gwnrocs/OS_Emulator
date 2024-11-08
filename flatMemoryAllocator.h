#pragma once
#ifndef FLATMEMORYALLOCATOR_H
#define FLATMEMORYALLOCATOR_H

#include "IMemoryAllocator.h"
#include<iostream>
#include <vector>
#include <unordered_map>


class FlatMemoryAllocator : public IMemoryAllocator {
public:
    FlatMemoryAllocator(size_t maximumSize);
    ~FlatMemoryAllocator();

    void* allocate(size_t size) override;
    void deallocate(void* ptr) override;
    std::string visualizeMemory() override;
    void initializeMemory();
    bool canAllocateAt(size_t index, size_t size);

private:
    size_t maximumSize;
    size_t allocatedSize;
    size_t memPerFrame = 0;
    size_t numFrames = 0;
    std::vector<char> memory;
    std::unordered_map<size_t, bool> allocationMap;

    //void initializeMemory();
    //bool canAllocateAt(size_t index, size_t size);
    //void allocateAt(size_t index, size_t size);
    //void deallocateAt(size_t index);
};
#endif