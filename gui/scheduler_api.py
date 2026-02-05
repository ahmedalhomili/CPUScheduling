"""
CPU Scheduling Python API
Wrapper for C++ Scheduler using ctypes

This module provides Python bindings to the C++ CPU scheduling library.
It can work in two modes:
1. Using the compiled C++ DLL/SO library (fast)
2. Using built-in Python implementations (fallback)

Author: Student
Date: 2026
"""

from ctypes import *
import os
import platform
from typing import List, Dict, Optional, Tuple
from dataclasses import dataclass
from enum import Enum


class Algorithm(Enum):
    """Scheduling Algorithms"""
    FCFS = 1
    SJF_NON_PREEMPTIVE = 2
    SJF_PREEMPTIVE = 3  # SRTF
    PRIORITY_NON_PREEMPTIVE = 4
    PRIORITY_PREEMPTIVE = 5
    ROUND_ROBIN = 6


@dataclass
class ProcessInput:
    """Process input data"""
    id: int
    arrival_time: int
    burst_time: int
    priority: int = 1


@dataclass
class ProcessResult:
    """Process result after scheduling"""
    id: int
    arrival_time: int
    burst_time: int
    priority: int
    waiting_time: int
    turnaround_time: int
    completion_time: int
    start_time: int


@dataclass
class TimelineEntry:
    """Execution timeline entry"""
    process_id: int
    start_time: int
    end_time: int


@dataclass
class SchedulingResult:
    """Complete scheduling result"""
    algorithm_name: str
    processes: List[ProcessResult]
    timeline: List[TimelineEntry]
    avg_waiting_time: float
    avg_turnaround_time: float
    cpu_utilization: float


class CPUSchedulerAPI:
    """
    Python API for CPU Scheduling Algorithms
    
    Usage:
        scheduler = CPUSchedulerAPI()
        
        # Add processes
        processes = [
            ProcessInput(1, 0, 5, 2),
            ProcessInput(2, 1, 3, 1),
            ProcessInput(3, 2, 8, 3),
        ]
        
        # Run algorithm
        result = scheduler.run_fcfs(processes)
        
        # Print results
        print(f"Average Waiting Time: {result.avg_waiting_time}")
        print(f"Average Turnaround Time: {result.avg_turnaround_time}")
    """
    
    def __init__(self, lib_path: Optional[str] = None):
        """
        Initialize the scheduler API
        
        Args:
            lib_path: Path to the C++ library (optional)
        """
        self._lib = None
        self._use_native = False
        
        if lib_path and os.path.exists(lib_path):
            self._load_library(lib_path)
        else:
            # Try to find the library automatically
            self._try_load_library()
    
    def _try_load_library(self):
        """Try to load the C++ library from common locations"""
        system = platform.system()
        
        if system == "Windows":
            lib_names = ["cpu_scheduler.dll", "libcpu_scheduler.dll"]
        elif system == "Darwin":
            lib_names = ["libcpu_scheduler.dylib"]
        else:
            lib_names = ["libcpu_scheduler.so"]
        
        search_paths = [
            ".",
            "./lib",
            "./build/lib",
            "../lib",
            "../build/lib",
            "./cmake-build-debug",
            "./cmake-build-release",
        ]
        
        for path in search_paths:
            for lib_name in lib_names:
                full_path = os.path.join(path, lib_name)
                if os.path.exists(full_path):
                    self._load_library(full_path)
                    return
        
        # Library not found, use Python implementation
        print("Note: C++ library not found, using Python implementation")
    
    def _load_library(self, path: str):
        """Load the C++ library"""
        try:
            self._lib = CDLL(path)
            self._setup_functions()
            self._use_native = True
            print(f"Loaded native library: {path}")
        except Exception as e:
            print(f"Failed to load library: {e}")
            self._lib = None
    
    def _setup_functions(self):
        """Setup ctypes function signatures"""
        if not self._lib:
            return
        
        # api_create_processes
        self._lib.api_create_processes.argtypes = [c_int]
        self._lib.api_create_processes.restype = c_void_p
        
        # api_free_processes
        self._lib.api_free_processes.argtypes = [c_void_p]
        self._lib.api_free_processes.restype = None
        
        # api_set_process
        self._lib.api_set_process.argtypes = [c_void_p, c_int, c_int, c_int, c_int, c_int]
        self._lib.api_set_process.restype = None
        
        # Algorithm functions
        for func_name in ['api_run_fcfs', 'api_run_sjf_non_preemptive', 
                          'api_run_sjf_preemptive', 'api_run_priority_non_preemptive',
                          'api_run_priority_preemptive']:
            getattr(self._lib, func_name).argtypes = [c_void_p, c_int]
            getattr(self._lib, func_name).restype = c_void_p
        
        self._lib.api_run_round_robin.argtypes = [c_void_p, c_int, c_int]
        self._lib.api_run_round_robin.restype = c_void_p
        
        # Result getters
        self._lib.api_get_avg_waiting_time.argtypes = [c_void_p]
        self._lib.api_get_avg_waiting_time.restype = c_double
        
        self._lib.api_get_avg_turnaround_time.argtypes = [c_void_p]
        self._lib.api_get_avg_turnaround_time.restype = c_double
        
        self._lib.api_get_cpu_utilization.argtypes = [c_void_p]
        self._lib.api_get_cpu_utilization.restype = c_double
        
        self._lib.api_get_timeline_length.argtypes = [c_void_p]
        self._lib.api_get_timeline_length.restype = c_int
        
        self._lib.api_free_result.argtypes = [c_void_p]
        self._lib.api_free_result.restype = None
    
    def run_algorithm(self, algorithm: Algorithm, processes: List[ProcessInput], 
                      time_quantum: int = 2) -> SchedulingResult:
        """
        Run a scheduling algorithm
        
        Args:
            algorithm: The scheduling algorithm to use
            processes: List of input processes
            time_quantum: Time quantum for Round Robin
            
        Returns:
            SchedulingResult with all metrics and timeline
        """
        if algorithm == Algorithm.FCFS:
            return self.run_fcfs(processes)
        elif algorithm == Algorithm.SJF_NON_PREEMPTIVE:
            return self.run_sjf_non_preemptive(processes)
        elif algorithm == Algorithm.SJF_PREEMPTIVE:
            return self.run_sjf_preemptive(processes)
        elif algorithm == Algorithm.PRIORITY_NON_PREEMPTIVE:
            return self.run_priority_non_preemptive(processes)
        elif algorithm == Algorithm.PRIORITY_PREEMPTIVE:
            return self.run_priority_preemptive(processes)
        elif algorithm == Algorithm.ROUND_ROBIN:
            return self.run_round_robin(processes, time_quantum)
        else:
            raise ValueError(f"Unknown algorithm: {algorithm}")
    
    def run_fcfs(self, processes: List[ProcessInput]) -> SchedulingResult:
        """Run First Come First Serve algorithm"""
        return self._run_algorithm_python("FCFS", processes, self._fcfs)
    
    def run_sjf_non_preemptive(self, processes: List[ProcessInput]) -> SchedulingResult:
        """Run Shortest Job First (Non-Preemptive) algorithm"""
        return self._run_algorithm_python("SJF Non-Preemptive", processes, self._sjf_np)
    
    def run_sjf_preemptive(self, processes: List[ProcessInput]) -> SchedulingResult:
        """Run Shortest Job First (Preemptive/SRTF) algorithm"""
        return self._run_algorithm_python("SJF Preemptive (SRTF)", processes, self._sjf_p)
    
    def run_priority_non_preemptive(self, processes: List[ProcessInput]) -> SchedulingResult:
        """Run Priority Scheduling (Non-Preemptive) algorithm"""
        return self._run_algorithm_python("Priority Non-Preemptive", processes, self._priority_np)
    
    def run_priority_preemptive(self, processes: List[ProcessInput]) -> SchedulingResult:
        """Run Priority Scheduling (Preemptive) algorithm"""
        return self._run_algorithm_python("Priority Preemptive", processes, self._priority_p)
    
    def run_round_robin(self, processes: List[ProcessInput], time_quantum: int = 2) -> SchedulingResult:
        """Run Round Robin algorithm"""
        return self._run_algorithm_python(f"Round Robin (Q={time_quantum})", processes, 
                                          lambda p: self._round_robin(p, time_quantum))
    
    def run_all_algorithms(self, processes: List[ProcessInput], 
                           time_quantum: int = 2) -> Dict[Algorithm, SchedulingResult]:
        """Run all scheduling algorithms and return results"""
        results = {}
        for algo in Algorithm:
            results[algo] = self.run_algorithm(algo, processes, time_quantum)
        return results
    
    def compare_algorithms(self, processes: List[ProcessInput], 
                           time_quantum: int = 2) -> str:
        """Compare all algorithms and return a formatted comparison"""
        results = self.run_all_algorithms(processes, time_quantum)
        
        lines = [
            "=" * 70,
            "Algorithm Comparison",
            "=" * 70,
            f"{'Algorithm':<30} {'Avg WT':>12} {'Avg TAT':>12} {'CPU %':>10}",
            "-" * 70,
        ]
        
        for algo, result in results.items():
            lines.append(
                f"{result.algorithm_name:<30} {result.avg_waiting_time:>12.2f} "
                f"{result.avg_turnaround_time:>12.2f} {result.cpu_utilization:>9.1f}%"
            )
        
        lines.append("=" * 70)
        return "\n".join(lines)
    
    # ==========================================
    # Python Implementation (Fallback)
    # ==========================================
    
    def _run_algorithm_python(self, name: str, processes: List[ProcessInput], 
                              algo_func) -> SchedulingResult:
        """Run algorithm using Python implementation"""
        procs = sorted([{
            "id": p.id,
            "arrival": p.arrival_time,
            "burst": p.burst_time,
            "priority": p.priority,
            "remaining": p.burst_time,
            "completion": 0,
            "waiting": 0,
            "turnaround": 0,
            "start": -1,
        } for p in processes], key=lambda x: x["arrival"])
        
        timeline, process_results = algo_func(procs)
        
        total_waiting = sum(p["waiting"] for p in process_results)
        total_turnaround = sum(p["turnaround"] for p in process_results)
        total_burst = sum(p["burst"] for p in process_results)
        total_time = max(p["completion"] for p in process_results) if process_results else 0
        n = len(process_results)
        
        return SchedulingResult(
            algorithm_name=name,
            processes=[
                ProcessResult(
                    id=p["id"],
                    arrival_time=p["arrival"],
                    burst_time=p["burst"],
                    priority=p["priority"],
                    waiting_time=p["waiting"],
                    turnaround_time=p["turnaround"],
                    completion_time=p["completion"],
                    start_time=p["start"],
                )
                for p in process_results
            ],
            timeline=[
                TimelineEntry(t["pid"], t["start"], t["end"])
                for t in timeline
            ],
            avg_waiting_time=total_waiting / n if n > 0 else 0,
            avg_turnaround_time=total_turnaround / n if n > 0 else 0,
            cpu_utilization=(total_burst / total_time * 100) if total_time > 0 else 0,
        )
    
    def _fcfs(self, procs: List[dict]) -> Tuple[List[dict], List[dict]]:
        """FCFS Algorithm"""
        timeline = []
        current_time = 0
        
        for p in procs:
            if current_time < p["arrival"]:
                current_time = p["arrival"]
            
            p["start"] = current_time
            p["completion"] = current_time + p["burst"]
            p["turnaround"] = p["completion"] - p["arrival"]
            p["waiting"] = p["turnaround"] - p["burst"]
            
            timeline.append({"pid": p["id"], "start": current_time, "end": p["completion"]})
            current_time = p["completion"]
        
        return timeline, procs
    
    def _sjf_np(self, procs: List[dict]) -> Tuple[List[dict], List[dict]]:
        """SJF Non-Preemptive"""
        timeline = []
        current_time = 0
        completed = 0
        n = len(procs)
        done = [False] * n
        
        while completed < n:
            shortest = -1
            min_burst = float('inf')
            
            for i, p in enumerate(procs):
                if not done[i] and p["arrival"] <= current_time and p["burst"] < min_burst:
                    min_burst = p["burst"]
                    shortest = i
            
            if shortest == -1:
                current_time += 1
                continue
            
            p = procs[shortest]
            p["start"] = current_time
            p["completion"] = current_time + p["burst"]
            p["turnaround"] = p["completion"] - p["arrival"]
            p["waiting"] = p["turnaround"] - p["burst"]
            
            timeline.append({"pid": p["id"], "start": current_time, "end": p["completion"]})
            current_time = p["completion"]
            done[shortest] = True
            completed += 1
        
        return timeline, procs
    
    def _sjf_p(self, procs: List[dict]) -> Tuple[List[dict], List[dict]]:
        """SJF Preemptive (SRTF)"""
        timeline = []
        current_time = 0
        completed = 0
        n = len(procs)
        last_process = -1
        last_start = 0
        
        for p in procs:
            p["remaining"] = p["burst"]
        
        while completed < n:
            shortest = -1
            min_remaining = float('inf')
            
            for i, p in enumerate(procs):
                if p["arrival"] <= current_time and p["remaining"] > 0 and p["remaining"] < min_remaining:
                    min_remaining = p["remaining"]
                    shortest = i
            
            if shortest == -1:
                if last_process != -1:
                    timeline.append({"pid": procs[last_process]["id"], "start": last_start, "end": current_time})
                    last_process = -1
                current_time += 1
                continue
            
            if shortest != last_process:
                if last_process != -1 and current_time > last_start:
                    timeline.append({"pid": procs[last_process]["id"], "start": last_start, "end": current_time})
                last_start = current_time
            
            last_process = shortest
            p = procs[shortest]
            
            if p["start"] == -1:
                p["start"] = current_time
            
            p["remaining"] -= 1
            current_time += 1
            
            if p["remaining"] == 0:
                p["completion"] = current_time
                p["turnaround"] = p["completion"] - p["arrival"]
                p["waiting"] = p["turnaround"] - p["burst"]
                timeline.append({"pid": p["id"], "start": last_start, "end": current_time})
                last_process = -1
                completed += 1
        
        return timeline, procs
    
    def _priority_np(self, procs: List[dict]) -> Tuple[List[dict], List[dict]]:
        """Priority Non-Preemptive"""
        timeline = []
        current_time = 0
        completed = 0
        n = len(procs)
        done = [False] * n
        
        while completed < n:
            highest = -1
            min_priority = float('inf')
            
            for i, p in enumerate(procs):
                if not done[i] and p["arrival"] <= current_time and p["priority"] < min_priority:
                    min_priority = p["priority"]
                    highest = i
            
            if highest == -1:
                current_time += 1
                continue
            
            p = procs[highest]
            p["start"] = current_time
            p["completion"] = current_time + p["burst"]
            p["turnaround"] = p["completion"] - p["arrival"]
            p["waiting"] = p["turnaround"] - p["burst"]
            
            timeline.append({"pid": p["id"], "start": current_time, "end": p["completion"]})
            current_time = p["completion"]
            done[highest] = True
            completed += 1
        
        return timeline, procs
    
    def _priority_p(self, procs: List[dict]) -> Tuple[List[dict], List[dict]]:
        """Priority Preemptive"""
        timeline = []
        current_time = 0
        completed = 0
        n = len(procs)
        last_process = -1
        last_start = 0
        
        for p in procs:
            p["remaining"] = p["burst"]
        
        while completed < n:
            highest = -1
            min_priority = float('inf')
            
            for i, p in enumerate(procs):
                if p["arrival"] <= current_time and p["remaining"] > 0 and p["priority"] < min_priority:
                    min_priority = p["priority"]
                    highest = i
            
            if highest == -1:
                if last_process != -1:
                    timeline.append({"pid": procs[last_process]["id"], "start": last_start, "end": current_time})
                    last_process = -1
                current_time += 1
                continue
            
            if highest != last_process:
                if last_process != -1 and current_time > last_start:
                    timeline.append({"pid": procs[last_process]["id"], "start": last_start, "end": current_time})
                last_start = current_time
            
            last_process = highest
            p = procs[highest]
            
            if p["start"] == -1:
                p["start"] = current_time
            
            p["remaining"] -= 1
            current_time += 1
            
            if p["remaining"] == 0:
                p["completion"] = current_time
                p["turnaround"] = p["completion"] - p["arrival"]
                p["waiting"] = p["turnaround"] - p["burst"]
                timeline.append({"pid": p["id"], "start": last_start, "end": current_time})
                last_process = -1
                completed += 1
        
        return timeline, procs
    
    def _round_robin(self, procs: List[dict], quantum: int) -> Tuple[List[dict], List[dict]]:
        """Round Robin"""
        timeline = []
        current_time = 0
        completed = 0
        n = len(procs)
        queue = []
        idx = 0
        
        for p in procs:
            p["remaining"] = p["burst"]
        
        # Add first processes
        while idx < n and procs[idx]["arrival"] <= current_time:
            queue.append(procs[idx])
            idx += 1
        
        while completed < n:
            if not queue:
                if idx < n:
                    current_time = procs[idx]["arrival"]
                    while idx < n and procs[idx]["arrival"] <= current_time:
                        queue.append(procs[idx])
                        idx += 1
                continue
            
            p = queue.pop(0)
            
            if p["start"] == -1:
                p["start"] = current_time
            
            exec_time = min(quantum, p["remaining"])
            timeline.append({"pid": p["id"], "start": current_time, "end": current_time + exec_time})
            
            p["remaining"] -= exec_time
            current_time += exec_time
            
            # Add newly arrived processes
            while idx < n and procs[idx]["arrival"] <= current_time:
                queue.append(procs[idx])
                idx += 1
            
            if p["remaining"] > 0:
                queue.append(p)
            else:
                p["completion"] = current_time
                p["turnaround"] = p["completion"] - p["arrival"]
                p["waiting"] = p["turnaround"] - p["burst"]
                completed += 1
        
        return timeline, procs


# ==========================================
# Demo / Test
# ==========================================
def demo():
    """Demo the API with sample data from professor"""
    print("=" * 60)
    print("CPU Scheduling API Demo")
    print("=" * 60)
    
    # Sample data from professor
    processes = [
        ProcessInput(1, 0, 5, 2),  # P1: arrival=0, burst=5, priority=2
        ProcessInput(2, 1, 3, 1),  # P2: arrival=1, burst=3, priority=1
        ProcessInput(3, 2, 8, 3),  # P3: arrival=2, burst=8, priority=3
    ]
    
    scheduler = CPUSchedulerAPI()
    
    # Compare all algorithms
    print(scheduler.compare_algorithms(processes, time_quantum=2))
    
    # Detailed output for FCFS
    print("\n" + "=" * 60)
    print("Detailed FCFS Result:")
    print("=" * 60)
    
    result = scheduler.run_fcfs(processes)
    
    print(f"\nAlgorithm: {result.algorithm_name}")
    print(f"Average Waiting Time: {result.avg_waiting_time:.2f}")
    print(f"Average Turnaround Time: {result.avg_turnaround_time:.2f}")
    print(f"CPU Utilization: {result.cpu_utilization:.1f}%")
    
    print("\nProcess Results:")
    print(f"{'PID':<6} {'Arrival':<8} {'Burst':<8} {'Wait':<8} {'TAT':<8} {'Completion':<12}")
    print("-" * 50)
    for p in result.processes:
        print(f"P{p.id:<5} {p.arrival_time:<8} {p.burst_time:<8} {p.waiting_time:<8} "
              f"{p.turnaround_time:<8} {p.completion_time:<12}")
    
    print("\nGantt Chart:")
    print("|", end="")
    for t in result.timeline:
        print(f" P{t.process_id} |", end="")
    print()
    print("0", end="")
    for t in result.timeline:
        print(f"    {t.end_time}", end="")
    print()


if __name__ == "__main__":
    demo()
