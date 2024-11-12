#include "flatMemoryAllocator.h"

FlatMemoryAllocator::FlatMemoryAllocator(size_t maximumSize) : maximumSize(maximumSize), allocatedSize(0) {
	memory.reserve(maximumSize);
	initializeMemory();
}

FlatMemoryAllocator::~FlatMemoryAllocator() {
	memory.clear();
}

void* FlatMemoryAllocator::allocate(size_t size) 
{
	// Find the first available block that can accomodate the process
	for (size_t i = 0; i < maximumSize - size + 1; ++i)
	{
		if (!allocationMap[i] && canAllocateAt(i, size)) 
		{
			//allocateAt(i, size);
			for (size_t j = i; j < i + size; i++)
			{
				allocationMap[j] = true;
			}
			allocatedSize += size;
			return &memory[i];
		}
	}

	//no available block found, return null pointer
	return nullptr;
}

void FlatMemoryAllocator::deallocate(void* ptr)
{
	// Find the index of the memory block to deallocate
	size_t index = static_cast<char*>(ptr) - &memory[0];
	if (allocationMap[index]) 
	{
		allocationMap[index] = false;
	}
}

std::string FlatMemoryAllocator::visualizeMemory()
{
	return std::string(memory.begin(), memory.end());
}

void FlatMemoryAllocator::initializeMemory() {
	//std::fill(memory.begin(), memory.end(), '.'); // '.' represents unallocated memory
	//std::fill(allocationMap.begin(), allocationMap.end(), false);

	for (size_t i = 0; i < maximumSize; ++i) {
		memory[i] = '.'; // '.' represents unallocated memory
		allocationMap[0] = false;
	}
}

bool FlatMemoryAllocator::canAllocateAt(size_t index, size_t size) {
	//check if the memory block is large enough
	return (index + size <= maximumSize);
}

//void FlatMemoryAllocator::allocateAt(size_t index, size_t size) 
//{
//	//Mark the memory block as allocated
//	//std::fill(allocationMap.begin() + index, allocationMap.begin() + index + size, true);
//	for (size_t i = index; i < index + size; i++)
//	{
//		allocationMap[i] = true;
//	}
//	allocatedSize += size;
//}
//
//void FlatMemoryAllocator::deallocateAt(size_t index) {
//	//Mark the memory block as deallocated
//	allocationMap[index] = false;
//}