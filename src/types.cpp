/**
 * @file types.cpp
 * @brief Definition of basic types, constants and their functions
 */

#ifndef TYPES_CPP_INCLUDED
#define TYPES_CPP_INCLUDED

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

// ==========================================
// Constants
// ==========================================
const int MAX_PROCESSES = 100;
const int MAX_TIMELINE = 1000;
const int DEFAULT_TIME_QUANTUM = 2;

// ==========================================
// Process Structure
// ==========================================
struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
    int start_time;
    bool is_completed;
};

// ==========================================
// Execution Record
// ==========================================
struct ExecutionRecord {
    int process_id;
    int start_time;
    int end_time;
};

// ==========================================
// Scheduling Result
// ==========================================
struct SchedulingResult {
    char algorithm_name[64];
    Process processes[MAX_PROCESSES];
    int process_count;
    ExecutionRecord timeline[MAX_TIMELINE];
    int timeline_length;
    double avg_waiting_time;
    double avg_turnaround_time;
    double cpu_utilization;
};

// ==========================================
// Process Functions
// ==========================================

// Create new process
Process create_process(int id, int arrival, int burst, int priority = 1) {
    Process p;
    p.id = id;
    p.arrival_time = arrival;
    p.burst_time = burst;
    p.priority = priority;
    p.remaining_time = burst;
    p.waiting_time = 0;
    p.turnaround_time = 0;
    p.completion_time = 0;
    p.start_time = -1;
    p.is_completed = false;
    return p;
}

// Reset process
void reset_process(Process& p) {
    p.remaining_time = p.burst_time;
    p.waiting_time = 0;
    p.turnaround_time = 0;
    p.completion_time = 0;
    p.start_time = -1;
    p.is_completed = false;
}

#endif // TYPES_CPP_INCLUDED
