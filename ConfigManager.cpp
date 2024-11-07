#include "ConfigManager.h"
#include <limits>

namespace Config {
    // Define the global configParams instance
    ConfigParams configParams;

    constexpr unsigned long long MAX_VALUE = 4294967296;  // 2^32

    /* Loads and validates the configuration from the config file */
    bool loadConfig(const string& filename) {
        ifstream configFile(filename);
        if (!configFile) {
            cout << "\nError opening " << filename << " file!" << endl;
            return false;
        }

        unsigned int    temp_num_cpu, temp_quantum_cycles, temp_min_ins, temp_max_ins, temp_batch_freq, 
                        temp_delays_per_exec, temp_max_overall_mem, temp_mem_per_frame, temp_min_mem_per_proc, temp_max_mem_per_proc;
        string temp_scheduler;
        const unsigned long long maxValue = 4294967296; // 2^32
        bool valid = true;
        string param;

        while (configFile >> param) {
            if (param == "num-cpu") {
                configFile >> temp_num_cpu;
                if (temp_num_cpu <= 0 || temp_num_cpu > 128) {
                    cout << "\n  Invalid value for num-cpu: " << temp_num_cpu << ". Must be within range [1, 128]." << endl;
                    valid = false; break;
                }
            }
            else if (param == "scheduler") {
                configFile >> temp_scheduler;
                if (temp_scheduler != "fcfs" && temp_scheduler != "rr") {
                    cout << "\n  Invalid value for scheduler: " << temp_scheduler << ". Must be 'fcfs' or 'rr'." << endl;
                    valid = false; break;
                }
            }
            else if (param == "quantum-cycles") {
                configFile >> temp_quantum_cycles;
                if (temp_quantum_cycles < 1 || temp_quantum_cycles > maxValue) {
                    cout << "\n  Invalid value for quantum-cycles: " << temp_quantum_cycles << ". Must be in range [1, 2^32]." << endl;
                    valid = false; break;
                }
            }
            else if (param == "min-ins") {
                configFile >> temp_min_ins;
                if (temp_min_ins < 1 || temp_min_ins > maxValue) {
                    cout << "\n  Invalid value for min-ins: " << temp_min_ins << ". Must be in range [1, 2^32]." << endl;
                    valid = false; break;
                }
            }
            else if (param == "max-ins") {
                configFile >> temp_max_ins;
                if (temp_max_ins < 1 || temp_max_ins > maxValue || temp_max_ins < temp_min_ins) {
                    cout << "\n  Invalid value for max-ins: " << temp_max_ins << ". Must be in range [1, 2^32] and more than min-ins." << endl;
                    valid = false; break;
                }
            }
            else if (param == "batch-process-freq") {
                configFile >> temp_batch_freq;
                if (temp_batch_freq < 1 || temp_batch_freq > maxValue) {
                    cout << "\n  Invalid value for batch-process-freq: " << temp_batch_freq << ". Must be in range [1, 2^32]." << endl;
                    valid = false; break;
                }
            }
            else if (param == "delays-per-exec") {
                configFile >> temp_delays_per_exec;
                if (temp_delays_per_exec < 0 || temp_delays_per_exec > maxValue) {
                    cout << "\n  Invalid value for delays-per-exec: " << temp_delays_per_exec << ". Must be in range [0, 2^32]." << endl;
                    valid = false; break;
                }
            }
            else if (param == "max-overall-mem") {
                configFile >> temp_max_overall_mem;
                if (temp_max_overall_mem <= 0 || temp_max_overall_mem > maxValue) {
                    cout << "\n  Invalid value for max-overall-mem: " << temp_max_overall_mem << ". Must be in range [1, 2^32]." << endl;
                    valid = false; break;
                }
            }
            else if (param == "mem-per-frame") {
                configFile >> temp_mem_per_frame;
                if (temp_mem_per_frame <= 0 || temp_mem_per_frame > maxValue) {
                    cout << "\n  Invalid value for mem-per-frame: " << temp_mem_per_frame << ". Must be in range [1, 2^32]." << endl;
                    valid = false; break;
                }
            }
            else if (param == "min-mem-per-proc") {
                configFile >> temp_min_mem_per_proc;
                if (temp_min_mem_per_proc <= 0 || temp_min_mem_per_proc > maxValue) {
                    cout << "\n  Invalid value for min-mem-per-proc: " << temp_min_mem_per_proc << ". Must be in range [1, 2^32]." << endl;
                    valid = false; break;
                }
            }
            else if (param == "max-mem-per-proc") {
                configFile >> temp_max_mem_per_proc;
                if (temp_max_mem_per_proc <= 0 || temp_max_mem_per_proc > maxValue || temp_max_mem_per_proc < temp_min_mem_per_proc) {
                    cout << "\n  Invalid value for max-mem-per-proc: " << temp_max_mem_per_proc << ". Must be in range [1, 2^32] and greater than min-mem-per-proc." << endl;
                    valid = false; break;
                }
            }
    }

        configFile.close();

        if (valid) {
            configParams.num_cpu = temp_num_cpu;
            configParams.scheduler_type = temp_scheduler;
            configParams.quantum_cycles = temp_quantum_cycles;
            configParams.min_ins = temp_min_ins;
            configParams.max_ins = temp_max_ins;
            configParams.batch_freq = temp_batch_freq;
            configParams.delays_per_exec = temp_delays_per_exec;
            configParams.max_overall_mem = temp_max_overall_mem;
            configParams.mem_per_frame = temp_mem_per_frame;
            configParams.min_mem_per_proc = temp_min_mem_per_proc;
            configParams.max_mem_per_proc = temp_max_mem_per_proc;
            return true;
        } else {
            cout << "Configuration loading failed due to invalid inputs.\nClearing screen . . ." << endl;
            return false;
        }
    }

} // namespace Config
