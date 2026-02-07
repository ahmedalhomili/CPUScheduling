/**
 * @file fcfs.cpp
 * @brief خوارزمية FCFS - First Come First Serve
 * 
 * أول عملية تصل هي أول عملية تُنفذ
 * خوارزمية بسيطة وغير استباقية
 */

#ifndef FCFS_CPP_INCLUDED
#define FCFS_CPP_INCLUDED

#include <cstring>

#include "../data_structures/queue.cpp"

using namespace std;

// ==========================================
// FCFS - First Come First Serve
// ==========================================
SchedulingResult fcfs(Process processes[], int count) {
    SchedulingResult result;
    strcpy(result.algorithm_name, "FCFS (First Come First Serve)");
    result.process_count = count;
    result.timeline_length = 0;
    
    // نسخ العمليات
    for (int i = 0; i < count; i++) {
        result.processes[i] = processes[i];
        reset_process(result.processes[i]);
    }
    
    // ترتيب حسب وقت الوصول (Bubble Sort)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (result.processes[j].arrival_time > result.processes[j + 1].arrival_time) {
                Process temp = result.processes[j];
                result.processes[j] = result.processes[j + 1];
                result.processes[j + 1] = temp;
            }
        }
    }
    
    int current_time = 0;
    
    // تنفيذ كل عملية بالترتيب
    for (int i = 0; i < count; i++) {
        // انتظار وصول العملية
        if (current_time < result.processes[i].arrival_time) {
            current_time = result.processes[i].arrival_time;
        }
        
        result.processes[i].start_time = current_time;
        result.processes[i].completion_time = current_time + result.processes[i].burst_time;
        result.processes[i].turnaround_time = result.processes[i].completion_time - result.processes[i].arrival_time;
        result.processes[i].waiting_time = result.processes[i].turnaround_time - result.processes[i].burst_time;
        result.processes[i].is_completed = true;
        
        // إضافة للجدول الزمني
        result.timeline[result.timeline_length].process_id = result.processes[i].id;
        result.timeline[result.timeline_length].start_time = current_time;
        result.timeline[result.timeline_length].end_time = result.processes[i].completion_time;
        result.timeline_length++;
        
        current_time = result.processes[i].completion_time;
    }
    
    return result;
}

#endif // FCFS_CPP_INCLUDED
