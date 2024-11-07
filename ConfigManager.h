#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace Config {

    // Struct to hold configuration parameters
    struct ConfigParams {
        int num_cpu;
        string scheduler_type;
        int quantum_cycles;
        int min_ins;
        int max_ins;
        int batch_freq;
        int delays_per_exec;
        unsigned int max_overall_mem;
        unsigned int mem_per_frame;
        unsigned int min_mem_per_proc;
        unsigned int max_mem_per_proc;
    };

    // Extern declaration for configParams to make it accessible globally
    extern ConfigParams configParams;

    // Function declarations for loading and validating configuration
    bool loadConfig(const string& filename);

    template <typename T>
    bool validateParam(ifstream& configFile, T& param, const T& min, const T& max, const string& paramName);

    template <typename T>
    bool validateRangeWithMin(ifstream& configFile, T& param, const T& min, const T& max, const string& paramName);

    bool validateScheduler(ifstream& configFile, string& scheduler);

} // namespace Config

#endif
