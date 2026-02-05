/**
 * @file api.cpp
 * @brief واجهة برمجية للتكامل مع Python/GUI
 * 
 * يوفر واجهة بسيطة لاستدعاء الخوارزميات من خارج C++
 */

#ifndef API_CPP_INCLUDED
#define API_CPP_INCLUDED

#include "io_handler.cpp"

using namespace std;

// ==========================================
// متغيرات عامة للAPI
// ==========================================
Process api_processes[MAX_PROCESSES];
int api_process_count = 0;
SchedulingResult api_last_result;

// ==========================================
// إعادة تهيئة
// ==========================================
void api_reset() {
    api_process_count = 0;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        api_processes[i] = Process();
    }
}

// ==========================================
// إضافة عملية
// ==========================================
bool api_add_process(int id, int arrival, int burst, int priority) {
    if (api_process_count >= MAX_PROCESSES) {
        return false;
    }
    api_processes[api_process_count] = create_process(id, arrival, burst, priority);
    api_process_count++;
    return true;
}

// ==========================================
// تحميل من ملف
// ==========================================
bool api_load_from_file(const char* filename) {
    api_reset();
    int count = load_processes_from_file(filename, api_processes);
    if (count > 0) {
        api_process_count = count;
        return true;
    }
    return false;
}

// ==========================================
// تشغيل خوارزمية
// ==========================================
SchedulingResult* api_run_algorithm(int algorithm_id, int time_quantum = DEFAULT_TIME_QUANTUM) {
    if (api_process_count == 0) {
        return nullptr;
    }
    
    switch (algorithm_id) {
        case 1:
            api_last_result = fcfs(api_processes, api_process_count);
            break;
        case 2:
            api_last_result = sjf_non_preemptive(api_processes, api_process_count);
            break;
        case 3:
            api_last_result = sjf_preemptive(api_processes, api_process_count);
            break;
        case 4:
            api_last_result = priority_non_preemptive(api_processes, api_process_count);
            break;
        case 5:
            api_last_result = priority_preemptive(api_processes, api_process_count);
            break;
        case 6:
            api_last_result = round_robin(api_processes, api_process_count, time_quantum);
            break;
        default:
            return nullptr;
    }
    
    return &api_last_result;
}

// ==========================================
// الحصول على النتيجة كـ JSON
// ==========================================
string api_get_result_json() {
    stringstream json;
    json << "{" << endl;
    json << "  \"algorithm\": \"" << api_last_result.algorithm_name << "\"," << endl;
    json << "  \"avg_waiting_time\": " << api_last_result.avg_waiting_time << "," << endl;
    json << "  \"avg_turnaround_time\": " << api_last_result.avg_turnaround_time << "," << endl;
    json << "  \"cpu_utilization\": " << api_last_result.cpu_utilization << "," << endl;
    
    json << "  \"processes\": [" << endl;
    for (int i = 0; i < api_last_result.process_count; i++) {
        const Process& p = api_last_result.processes[i];
        json << "    {";
        json << "\"id\": " << p.id << ", ";
        json << "\"arrival\": " << p.arrival_time << ", ";
        json << "\"burst\": " << p.burst_time << ", ";
        json << "\"priority\": " << p.priority << ", ";
        json << "\"start\": " << p.start_time << ", ";
        json << "\"completion\": " << p.completion_time << ", ";
        json << "\"waiting\": " << p.waiting_time << ", ";
        json << "\"turnaround\": " << p.turnaround_time;
        json << "}";
        if (i < api_last_result.process_count - 1) json << ",";
        json << endl;
    }
    json << "  ]," << endl;
    
    json << "  \"timeline\": [" << endl;
    for (int i = 0; i < api_last_result.timeline_length; i++) {
        json << "    {";
        json << "\"process_id\": " << api_last_result.timeline[i].process_id << ", ";
        json << "\"start\": " << api_last_result.timeline[i].start_time << ", ";
        json << "\"end\": " << api_last_result.timeline[i].end_time;
        json << "}";
        if (i < api_last_result.timeline_length - 1) json << ",";
        json << endl;
    }
    json << "  ]" << endl;
    
    json << "}" << endl;
    
    return json.str();
}

// ==========================================
// طباعة النتيجة
// ==========================================
void api_print_result() {
    print_result(api_last_result);
    print_gantt_chart(api_last_result);
}

// ==========================================
// تشغيل جميع الخوارزميات
// ==========================================
void api_run_all_algorithms(int time_quantum = DEFAULT_TIME_QUANTUM) {
    cout << "\n====== تشغيل جميع الخوارزميات ======\n" << endl;
    
    for (int algo = 1; algo <= 6; algo++) {
        api_run_algorithm(algo, time_quantum);
        api_print_result();
        cout << "\n" << string(50, '=') << "\n" << endl;
    }
}

#endif // API_CPP_INCLUDED
