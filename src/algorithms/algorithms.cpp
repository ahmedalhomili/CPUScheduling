/**
 * @file algorithms.cpp
 * @brief ملف تجميع خوارزميات جدولة العمليات
 * 
 * يتضمن 4 خوارزميات:
 * 1. FCFS - First Come First Serve
 * 2. SJF - Shortest Job First (Non-Preemptive & Preemptive)
 * 3. Priority - جدولة الأولوية (Non-Preemptive & Preemptive)
 * 4. Round Robin - الجدولة الدائرية
 */

#ifndef ALGORITHMS_CPP_INCLUDED
#define ALGORITHMS_CPP_INCLUDED

// تضمين الخوارزميات الأربعة
#include "fcfs.cpp"
#include "sjf.cpp"
#include "priority.cpp"
#include "round_robin.cpp"

using namespace std;

// ==========================================
// دالة حساب الإحصائيات
// ==========================================
void calculate_statistics(SchedulingResult& result) {
    double total_waiting = 0;
    double total_turnaround = 0;
    int total_burst = 0;
    int max_completion = 0;
    int min_arrival = (result.process_count > 0) ? result.processes[0].arrival_time : 0;
    
    for (int i = 0; i < result.process_count; i++) {
        total_waiting += result.processes[i].waiting_time;
        total_turnaround += result.processes[i].turnaround_time;
        total_burst += result.processes[i].burst_time;
        
        if (result.processes[i].completion_time > max_completion) {
            max_completion = result.processes[i].completion_time;
        }
        if (result.processes[i].arrival_time < min_arrival) {
            min_arrival = result.processes[i].arrival_time;
        }
    }
    
    result.avg_waiting_time = (result.process_count > 0) ? total_waiting / result.process_count : 0;
    result.avg_turnaround_time = (result.process_count > 0) ? total_turnaround / result.process_count : 0;
    
    int total_time = max_completion - min_arrival;
    result.cpu_utilization = (total_time > 0) ? (double)total_burst / total_time * 100.0 : 0;
}

#endif // ALGORITHMS_CPP_INCLUDED
