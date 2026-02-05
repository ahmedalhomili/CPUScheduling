/**
 * @file priority.cpp
 * @brief خوارزمية Priority Scheduling
 * 
 * جدولة حسب الأولوية (الرقم الأصغر = أولوية أعلى)
 * نسختين: Non-Preemptive و Preemptive
 */

#ifndef PRIORITY_CPP_INCLUDED
#define PRIORITY_CPP_INCLUDED

#include "../data_structures/linked_list.cpp"

using namespace std;

// ==========================================
// Priority Non-Preemptive
// ==========================================
SchedulingResult priority_non_preemptive(Process processes[], int count) {
    SchedulingResult result;
    strcpy(result.algorithm_name, "Priority (Non-Preemptive)");
    result.process_count = count;
    result.timeline_length = 0;
    
    for (int i = 0; i < count; i++) {
        result.processes[i] = processes[i];
        reset_process(result.processes[i]);
    }
    
    int current_time = 0;
    int completed = 0;
    
    while (completed < count) {
        // البحث عن العملية ذات الأولوية الأعلى (الرقم الأصغر)
        int highest_idx = -1;
        int highest_priority = 999999;
        
        for (int i = 0; i < count; i++) {
            if (!result.processes[i].is_completed &&
                result.processes[i].arrival_time <= current_time &&
                result.processes[i].priority < highest_priority) {
                highest_priority = result.processes[i].priority;
                highest_idx = i;
            }
        }
        
        if (highest_idx == -1) {
            current_time++;
            continue;
        }
        
        Process& p = result.processes[highest_idx];
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
// Priority Preemptive
// ==========================================
SchedulingResult priority_preemptive(Process processes[], int count) {
    SchedulingResult result;
    strcpy(result.algorithm_name, "Priority (Preemptive)");
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
        int highest_idx = -1;
        int highest_priority = 999999;
        
        for (int i = 0; i < count; i++) {
            if (!result.processes[i].is_completed &&
                result.processes[i].arrival_time <= current_time &&
                result.processes[i].priority < highest_priority) {
                highest_priority = result.processes[i].priority;
                highest_idx = i;
            }
        }
        
        if (highest_idx == -1) {
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
        
        if (last_process != highest_idx && last_process != -1) {
            result.timeline[result.timeline_length].process_id = result.processes[last_process].id;
            result.timeline[result.timeline_length].start_time = last_start;
            result.timeline[result.timeline_length].end_time = current_time;
            result.timeline_length++;
        }
        
        if (last_process != highest_idx) {
            last_start = current_time;
            last_process = highest_idx;
        }
        
        Process& p = result.processes[highest_idx];
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

#endif // PRIORITY_CPP_INCLUDED
