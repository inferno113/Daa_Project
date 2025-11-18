# Power-Aware Scheduler Simulation (Python)

import random

# VM Class with power model
class VM:
    def __init__(self, id, mips, power_idle, power_max):
        self.id = id
        self.mips = mips
        self.power_idle = power_idle
        self.power_max = power_max
        self.tasks = []

    def estimate_energy(self, task_length):
        utilization = task_length / self.mips
        utilization = min(utilization, 1.0)
        power = self.power_idle + (self.power_max - self.power_idle) * utilization
        time = task_length / self.mips
        return power * time, time

# Task (Cloudlet)
class Task:
    def __init__(self, id, length):
        self.id = id
        self.length = length

# Scheduler
class PowerAwareScheduler:
    def __init__(self, vms, tasks):
        self.vms = vms
        self.tasks = tasks
        self.schedule = []
        self.total_energy = 0
        self.total_time = 0

    def run(self):
        for task in self.tasks:
            best_vm = None
            min_energy = float('inf')
            best_time = 0

            for vm in self.vms:
                energy, time = vm.estimate_energy(task.length)
                if energy < min_energy:
                    min_energy = energy
                    best_time = time
                    best_vm = vm

            best_vm.tasks.append(task)
            self.schedule.append((task.id, best_vm.id, min_energy, best_time))
            self.total_energy += min_energy
            self.total_time += best_time

    def print_report(self):
        print("\nSchedule Report")
        print("Task ID | VM ID | Energy (J) | Time (s)")
        for t_id, vm_id, energy, time in self.schedule:
            print(f"   {t_id}    |  {vm_id}   |  {energy:.2f}     |  {time:.2f}")
        print("\nTotal Energy Used:", round(self.total_energy, 2), "Joules")
        print("Total Execution Time:", round(self.total_time, 2), "Seconds")

# Example Usage
def simulate():
    # Define VMs (id, mips, idle power, max power)
    vms = [
        VM(1, 1000, 50, 150),
        VM(2, 1500, 60, 170),
        VM(3, 800, 40, 130)
    ]

    # Generate Tasks (id, length)
    tasks = [Task(i, random.randint(500, 2000)) for i in range(1, 11)]

    scheduler = PowerAwareScheduler(vms, tasks)
    scheduler.run()
    scheduler.print_report()

if __name__ == "__main__":
    simulate()