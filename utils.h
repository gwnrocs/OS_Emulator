#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <random>
#include <sstream>
#include <windows.h>
#include <fstream>

using namespace std;

namespace Colors {
    const string Yellow = "\033[0;33m";
    const string Green = "\033[0;32m";
    const string Red = "\033[0;31m";
    const string White = "\033[0m";
}

namespace Utils {
    int generateRandomNumber(int lower, int upper);
    string getCurrentTimestamp();
    void printAscii();
    void printConfirmation(const string& command);
    void clearScreen();
    void printError(const string& command);
    void printHeader();
}

bool loadConfig(int& num_cpu, string& scheduler, int& quantum_cycles, int& min_ins, int& max_ins, int& batch_freq, int& delays_per_exec);
bool configFileExists();
#endif