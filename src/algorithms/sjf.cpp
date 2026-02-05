/**
 * @file sjf.cpp
 * @brief خوارزمية SJF - Shortest Job First
 * 
 * تنفيذ أقصر عملية أولاً
 * نسختين: Non-Preemptive و Preemptive (SRTF)
 */

#ifndef SJF_CPP_INCLUDED
#define SJF_CPP_INCLUDED

#include "../data_structures/priority_queue.cpp"

using namespace std;

// ==========================================
// SJF Non-Preemptive
// ==========================================
SchedulingResult sjf_non_preemptive(Process processes[], int count) {
    SchedulingResult result;
    strcpy(result.algorithm_name, "SJF (Non-Preemptive)");
    result.process_count = count;
    result.timeline_length = 0;
    
    for (int i = 0; i < count; i++) {
        result.processes[i] = processes[i];
        reset_process(result.processes[i]);
    }
    
    int current_time = 0;
    int completed = 0;
    
    while (completed < count) {
        // البحث عن أقصر عملية وصلت
        int shortest_idx = -1;
        int shortest_burst = 999999;
        
        for (int i = 0; i < count; i++) {
            if (!result.processes[i].is_completed &&
                result.processes[i].arrival_time <= current_time &&
                result.processes[i].burst_time < shortest_burst) {
                shortest_burst = result.processes[i].burst_time;
                shortest_idx = i;
            }
        }
        
        // لا توجد عملية متاحة
        if (shortest_idx == -1) {
            current_time++;
            continue;
        }
        
        Process& p = result.processes[shortest_idx];
        p.start_time = current_time;
        p.completion_time = current_time + p.burst_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
        p.is_completed = true;
        
        result.timeline[result.timeline_length].process_id = p.id;
        result.timeline[result.timeline_length].start_time = current_time;
        result.timeline[result.timeline_length].end_time = p.completion_time;
        result.timeline_length++;
        
        current_time = p.completion_time;
        completed++;
    }
    
    return result;
}

// ==========================================
// SJF Preemptive (SRTF)
// ==========================================
SchedulingResult sjf_preemptive(Process processes[], int count) {
    SchedulingResult result;
    strcpy(result.algorithm_name, "SJF Preemptive (SRTF)");
    result.process_count = count;
    result.timeline_length = 0;
    
    for (int i = 0; i < count; i++) {
        result.processes[i] = processes[i];
        reset_process(result.processes[i]);
    }
    
    int current_time = 0;
    int completed = 0;
    int last_process = -1;
    int last_start = 0;
    
    while (completed < count) {
        // البحث عن العملية ذات أقصر وقت متبقي
        int shortest_idx = -1;
        int shortest_remaining = 999999;
        
        for (int i = 0; i < count; i++) {
            if (!result.processes[i].is_completed &&
                result.processes[i].arrival_time <= current_time &&
                result.processes[i].remaining_time < shortest_remaining) {
                shortest_remaining = result.processes[i].remaining_time;
                shortest_idx = i;
            }
        }
        
        if (shortest_idx == -1) {
            if (last_process != -1) {
                result.timeline[result.timeline_length].process_id = result.processes[last_process].id;
                result.timeline[result.timeline_length].start_time = last_start;
                result.timeline[result.timeline_length].end_time = current_time;
                result.timeline_length++;
                last_process = -1;
            }
            current_time++;
            continue;
        }
        
        // تبديل العملية (preemption)
        if (last_process != shortest_idx && last_process != -1) {
            result.timeline[result.timeline_length].process_id = result.processes[last_process].id;
            result.timeline[result.timeline_length].start_time = last_start;
            result.timeline[result.timeline_length].end_time = current_time;
            result.timeline_length++;
        }
        
        if (last_process != shortest_idx) {
            last_start = current_time;
            last_process = shortest_idx;
        }
        
        Process& p = result.processes[shortest_idx];
        if (p.start_time == -1) {
            p.start_time = current_time;
        }
        
        p.remaining_time--;
        current_time++;
        
        if (p.remaining_time == 0) {
            p.completion_time = current_time;
            p.turnaround_time = p.completion_time - p.arrival_time;
            p.waiting_time = p.turnaround_time - p.burst_time;
            p.is_completed = true;
            completed++;
            
            result.timeline[result.timeline_length].process_id = p.id;
            result.timeline[result.timeline_length].start_time = last_start;
            result.timeline[result.timeline_length].end_time = current_time;
            result.timeline_length++;
            last_process = -1;
        }
    }
    
    return result;
}

#endif // SJF_CPP_INCLUDED
