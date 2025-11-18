#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <ctime>

using namespace std;

// Define structure for VM
struct VM {
    int id;
    double cpu_utilization;
    double frequency; // GHz
    bool is_active;

    VM(int id) : id(id), cpu_utilization(0), frequency(2.5), is_active(true) {}
};

// Define structure for Task
struct Task {
    int id;
    double size; // arbitrary unit
    double execution_time; // seconds
    double priority;

    Task(int id, double size) : id(id), size(size), execution_time(0), priority(0) {}
};

// Fuzzy Rule-Based System (Simple Approximation)
double calculatePriority(double cpu_util, double size) {
    if (cpu_util > 70 && size < 500)
        return 0.3;
    else if (cpu_util < 40 && size > 500)
        return 0.8;
    else
        return 0.5;
}

// Bat Algorithm Parameters
const double alpha = 0.9;
const double gamma = 0.9;
const double loudness_init = 1.0;
const double pulse_rate_init = 0.5;

// Bat Algorithm to Optimize Task-VM Mapping
void batAlgorithm(vector<Task>& tasks, vector<VM>& vms) {
    int n_bats = tasks.size();
    vector<int> best_assignment(n_bats, -1);
    double best_energy = 1e9;

    for (int iter = 0; iter < 100; iter++) {
        vector<int> assignment(n_bats);

        for (int i = 0; i < n_bats; i++) {
            assignment[i] = rand() % vms.size();
        }

        double total_energy = 0;
        for (int i = 0; i < n_bats; i++) {
            double load = vms[assignment[i]].cpu_utilization + tasks[i].size;
            total_energy += pow(load, 2);
        }

        if (total_energy < best_energy) {
            best_energy = total_energy;
            best_assignment = assignment;
        }
    }

    // Assign tasks according to best mapping
    for (int i = 0; i < n_bats; i++) {
        int vm_id = best_assignment[i];
        vms[vm_id].cpu_utilization += tasks[i].size;
        tasks[i].execution_time = tasks[i].size / vms[vm_id].frequency;
    }
}

// Apply DVFS Scaling
void applyDVFS(vector<VM>& vms) {
    for (auto& vm : vms) {
        if (vm.cpu_utilization > 800) {
            vm.frequency = 3.5;
        } else if (vm.cpu_utilization < 300) {
            vm.frequency = 1.5;
        } else {
            vm.frequency = 2.5;
        }
    }
}

// VM Consolidation
void consolidateVMs(vector<VM>& vms) {
    for (auto& vm : vms) {
        if (vm.cpu_utilization < 200) {
            vm.is_active = false;
            vm.cpu_utilization = 0;
        }
    }
}

int main() {
    srand(time(0));
    
    vector<VM> vms;
    for (int i = 0; i < 5; i++) {
        vms.push_back(VM(i));
    }

    vector<Task> tasks;
    for (int i = 0; i < 10; i++) {
        tasks.push_back(Task(i, rand() % 800 + 200));
        tasks[i].priority = calculatePriority(vms[i % 5].cpu_utilization, tasks[i].size);
    }

    sort(tasks.begin(), tasks.end(), [](Task& a, Task& b) {
        return a.priority > b.priority;
    });

    batAlgorithm(tasks, vms);
    applyDVFS(vms);
    consolidateVMs(vms);

    cout << "Final VM states after scheduling:\n";
    for (auto& vm : vms) {
        cout << "VM " << vm.id << ": Utilization=" << vm.cpu_utilization
             << ", Frequency=" << vm.frequency << " GHz"
             << ", Active=" << (vm.is_active ? "Yes" : "No") << endl;
    }

    return 0;
}
