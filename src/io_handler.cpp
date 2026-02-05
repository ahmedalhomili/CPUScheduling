/**
 * @file io_handler.cpp
 * @brief File and Print Handling
 * 
 * Contains:
 * - Load processes from file
 * - Print results
 * - Print Gantt chart
 */

#ifndef IO_HANDLER_CPP_INCLUDED
#define IO_HANDLER_CPP_INCLUDED

#include "algorithms/algorithms.cpp"
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

// ==========================================
// Load Processes from File
// ==========================================
int load_processes_from_file(const char* filename, Process processes[]) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return -1;
    }
    
    string line;
    int count = 0;
    
    // Skip header line
    getline(file, line);
    
    while (getline(file, line) && count < MAX_PROCESSES) {
        if (line.empty() || line[0] == '#') continue;
        
        stringstream ss(line);
        int id, arrival, burst, priority;
        char comma;
        
        // Read CSV format
        if (ss >> id >> comma >> arrival >> comma >> burst >> comma >> priority) {
            processes[count] = create_process(id, arrival, burst, priority);
            count++;
        } else {
            // Try reading without commas
            ss.clear();
            ss.str(line);
            if (ss >> id >> arrival >> burst >> priority) {
                processes[count] = create_process(id, arrival, burst, priority);
                count++;
            }
        }
    }
    
    file.close();
    return count;
}

// ==========================================
// Print Processes Information
// ==========================================
void print_processes(Process processes[], int count) {
    cout << "\n=== Input Processes ===" << endl;
    cout << "+-----+------------+------------+------------+" << endl;
    cout << "| ID  | Arrival    | Burst      | Priority   |" << endl;
    cout << "+-----+------------+------------+------------+" << endl;
    
    for (int i = 0; i < count; i++) {
        cout << "| " << setw(3) << processes[i].id << " | "
             << setw(10) << processes[i].arrival_time << " | "
             << setw(10) << processes[i].burst_time << " | "
             << setw(10) << processes[i].priority << " |" << endl;
    }
    cout << "+-----+------------+------------+------------+" << endl;
}

// ==========================================
// Print Results
// ==========================================
void print_result(const SchedulingResult& result) {
    cout << "\n========================================" << endl;
    cout << "Algorithm: " << result.algorithm_name << endl;
    cout << "========================================" << endl;
    
    cout << "\n--- Process Results ---" << endl;
    cout << "+-----+----------+----------+----------+----------+----------+" << endl;
    cout << "| ID  | Arrival  | Burst    | Start    | Complete | Wait     |" << endl;
    cout << "+-----+----------+----------+----------+----------+----------+" << endl;
    
    for (int i = 0; i < result.process_count; i++) {
        const Process& p = result.processes[i];
        cout << "| " << setw(3) << p.id << " | "
             << setw(8) << p.arrival_time << " | "
             << setw(8) << p.burst_time << " | "
             << setw(8) << p.start_time << " | "
             << setw(8) << p.completion_time << " | "
             << setw(8) << p.waiting_time << " |" << endl;
    }
    cout << "+-----+----------+----------+----------+----------+----------+" << endl;
    
    cout << "\n--- Statistics ---" << endl;
    cout << fixed << setprecision(2);
    cout << "Average Waiting Time: " << result.avg_waiting_time << endl;
    cout << "Average Turnaround Time: " << result.avg_turnaround_time << endl;
    cout << "CPU Utilization: " << result.cpu_utilization << "%" << endl;
}

// ==========================================
// Print Gantt Chart
// ==========================================
void print_gantt_chart(const SchedulingResult& result) {
    cout << "\n--- Gantt Chart ---" << endl;
    
    // Top line
    cout << "+";
    for (int i = 0; i < result.timeline_length; i++) {
        int duration = result.timeline[i].end_time - result.timeline[i].start_time;
        for (int j = 0; j < duration * 3; j++) cout << "-";
        cout << "+";
    }
    cout << endl;
    
    // Process names
    cout << "|";
    for (int i = 0; i < result.timeline_length; i++) {
        int duration = result.timeline[i].end_time - result.timeline[i].start_time;
        int spaces = duration * 3;
        int id_len = 2; // P + digit
        int left_pad = (spaces - id_len) / 2;
        int right_pad = spaces - id_len - left_pad;
        
        for (int j = 0; j < left_pad; j++) cout << " ";
        cout << "P" << result.timeline[i].process_id;
        for (int j = 0; j < right_pad; j++) cout << " ";
        cout << "|";
    }
    cout << endl;
    
    // Bottom line
    cout << "+";
    for (int i = 0; i < result.timeline_length; i++) {
        int duration = result.timeline[i].end_time - result.timeline[i].start_time;
        for (int j = 0; j < duration * 3; j++) cout << "-";
        cout << "+";
    }
    cout << endl;
    
    // Time markers
    cout << result.timeline[0].start_time;
    for (int i = 0; i < result.timeline_length; i++) {
        int duration = result.timeline[i].end_time - result.timeline[i].start_time;
        for (int j = 0; j < duration * 3; j++) cout << " ";
        cout << result.timeline[i].end_time;
    }
    cout << endl;
}

// ==========================================
// Print Menu
// ==========================================
void print_menu() {
    cout << "\n=== CPU Scheduling Simulator ===" << endl;
    cout << "--- First, load processes: ---" << endl;
    cout << "8. Load from File" << endl;
    cout << "9. Manual Input" << endl;
    cout << "--- Then, run an algorithm: ---" << endl;
    cout << "1. FCFS (First Come First Serve)" << endl;
    cout << "2. SJF Non-Preemptive" << endl;
    cout << "3. SJF Preemptive (SRTF)" << endl;
    cout << "4. Priority Non-Preemptive" << endl;
    cout << "5. Priority Preemptive" << endl;
    cout << "6. Round Robin" << endl;
    cout << "7. Run All Algorithms" << endl;
    cout << "--- Other options: ---" << endl;
    cout << "10. Data Structures Demo" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter choice: ";
}

// ==========================================
// Save Results to File
// ==========================================
bool save_result_to_file(const SchedulingResult& result, const char* filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file for writing" << endl;
        return false;
    }
    
    file << "Algorithm: " << result.algorithm_name << endl;
    file << "\nProcess Results:" << endl;
    file << "ID,Arrival,Burst,Start,Complete,Wait,Turnaround" << endl;
    
    for (int i = 0; i < result.process_count; i++) {
        const Process& p = result.processes[i];
        file << p.id << "," << p.arrival_time << "," << p.burst_time << ","
             << p.start_time << "," << p.completion_time << ","
             << p.waiting_time << "," << p.turnaround_time << endl;
    }
    
    file << "\nStatistics:" << endl;
    file << "Avg Waiting Time: " << result.avg_waiting_time << endl;
    file << "Avg Turnaround Time: " << result.avg_turnaround_time << endl;
    file << "CPU Utilization: " << result.cpu_utilization << "%" << endl;
    
    file << "\nTimeline:" << endl;
    for (int i = 0; i < result.timeline_length; i++) {
        file << "P" << result.timeline[i].process_id << " ["
             << result.timeline[i].start_time << "-"
             << result.timeline[i].end_time << "]" << endl;
    }
    
    file.close();
    cout << "Results saved to: " << filename << endl;
    return true;
}

#endif // IO_HANDLER_CPP_INCLUDED
