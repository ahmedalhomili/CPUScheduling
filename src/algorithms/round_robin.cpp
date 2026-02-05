/**
 * @file round_robin.cpp
 * @brief خوارزمية Round Robin
 * 
 * الجدولة الدائرية - كل عملية تأخذ شريحة زمنية (Time Quantum)
 * ثم تنتقل للعملية التالية في الطابور
 */

#ifndef ROUND_ROBIN_CPP_INCLUDED
#define ROUND_ROBIN_CPP_INCLUDED

#include "../data_structures/stack.cpp"

using namespace std;

// ==========================================
// Round Robin
// ==========================================
SchedulingResult round_robin(Process processes[], int count, int time_quantum = DEFAULT_TIME_QUANTUM) {
    SchedulingResult result;
    strcpy(result.algorithm_name, "Round Robin");
    result.process_count = count;
    result.timeline_length = 0;
    
    for (int i = 0; i < count; i++) {
        result.processes[i] = processes[i];
        reset_process(result.processes[i]);
    }
    
    // ترتيب حسب وقت الوصول
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (result.processes[j].arrival_time > result.processes[j + 1].arrival_time) {
                Process temp = result.processes[j];
                result.processes[j] = result.processes[j + 1];
                result.processes[j + 1] = temp;
            }
        }
    }
    
    // استخدام الطابور
    Queue* ready_queue = queue_create();
    int current_time = 0;
    int completed = 0;
    int next_arrival = 0;
    
    // إضافة العمليات الأولى
    while (next_arrival < count && result.processes[next_arrival].arrival_time <= current_time) {
        queue_enqueue(ready_queue, result.processes[next_arrival]);
        next_arrival++;
    }
    
    while (completed < count) {
        if (queue_is_empty(ready_queue)) {
            if (next_arrival < count) {
                current_time = result.processes[next_arrival].arrival_time;
                while (next_arrival < count && result.processes[next_arrival].arrival_time <= current_time) {
                    queue_enqueue(ready_queue, result.processes[next_arrival]);
                    next_arrival++;
                }
            } else {
                break;
            }
        }
        
        Process current = queue_dequeue(ready_queue);
        
        // البحث عن الفهرس في المصفوفة
        int idx = -1;
        for (int i = 0; i < count; i++) {
            if (result.processes[i].id == current.id) {
                idx = i;
                break;
            }
        }
        
        if (result.processes[idx].start_time == -1) {
            result.processes[idx].start_time = current_time;
        }
        
        // تنفيذ لمدة quantum أو حتى الانتهاء
        int exec_time = (current.remaining_time < time_quantum) ? current.remaining_time : time_quantum;
        
        result.timeline[result.timeline_length].process_id = current.id;
        result.timeline[result.timeline_length].start_time = current_time;
        result.timeline[result.timeline_length].end_time = current_time + exec_time;
        result.timeline_length++;
        
        current_time += exec_time;
        result.processes[idx].remaining_time -= exec_time;
        
        // إضافة العمليات الجديدة التي وصلت
        while (next_arrival < count && result.processes[next_arrival].arrival_time <= current_time) {
            queue_enqueue(ready_queue, result.processes[next_arrival]);
            next_arrival++;
        }
        
        // إعادة العملية للطابور إذا لم تنتهي
        if (result.processes[idx].remaining_time > 0) {
            Process updated = result.processes[idx];
            queue_enqueue(ready_queue, updated);
        } else {
            result.processes[idx].completion_time = current_time;
            result.processes[idx].turnaround_time = result.processes[idx].completion_time - result.processes[idx].arrival_time;
            result.processes[idx].waiting_time = result.processes[idx].turnaround_time - result.processes[idx].burst_time;
            result.processes[idx].is_completed = true;
            completed++;
        }
    }
    
    queue_destroy(ready_queue);
    return result;
}

#endif // ROUND_ROBIN_CPP_INCLUDED
