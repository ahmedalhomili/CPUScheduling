/**
 * @file main.cpp
 * @brief Main File - CPU Scheduling Simulator
 * 
 * @author Ahmed
 * @date 2024
 * 
 * Features:
 * - 6 scheduling algorithms
 * - Manual input or from file
 * - Gantt chart printing
 * - Statistics calculation
 */

#include "src/api.cpp"

using namespace std;

// ==========================================
// Manual Input Function
// ==========================================
void manual_input() {
    api_reset();
    
    int n;
    cout << "Number of processes: ";
    cin >> n;
    
    if (n <= 0 || n > MAX_PROCESSES) {
        cout << "Invalid number!" << endl;
        return;
    }
    
    cout << "\nEnter data for each process:" << endl;
    cout << "(Arrival Time, Burst Time, Priority)" << endl;
    
    for (int i = 0; i < n; i++) {
        int arrival, burst, priority;
        cout << "P" << (i + 1) << ": ";
        cin >> arrival >> burst >> priority;
        api_add_process(i + 1, arrival, burst, priority);
    }
    
    print_processes(api_processes, api_process_count);
}

// ==========================================
// Load from File Function
// ==========================================
bool load_from_file() {
    string filename;
    cout << "Filename: ";
    cin >> filename;
    
    if (api_load_from_file(filename.c_str())) {
        print_processes(api_processes, api_process_count);
        return true;
    }
    return false;
}

// ==========================================
// Run Single Algorithm
// ==========================================
void run_single_algorithm(int algo) {
    if (api_process_count == 0) {
        cout << "No processes! Please add processes first." << endl;
        return;
    }
    
    int quantum = DEFAULT_TIME_QUANTUM;
    if (algo == 6) {
        cout << "Time Quantum (default " << DEFAULT_TIME_QUANTUM << "): ";
        string input;
        cin.ignore();
        getline(cin, input);
        if (!input.empty()) {
            quantum = stoi(input);
        }
    }
    
    SchedulingResult* result = api_run_algorithm(algo, quantum);
    if (result) {
        api_print_result();
    }
}

// ==========================================
// Data Structures Demo
// ==========================================
void demo_data_structures() {
    cout << "\n=== Data Structures Demo ===\n";
    
    // Queue Demo
    cout << "\n--- Queue Demo (FIFO) ---\n";
    Queue* q = queue_create();
    queue_enqueue(q, create_process(1, 0, 5));
    queue_enqueue(q, create_process(2, 1, 3));
    queue_enqueue(q, create_process(3, 2, 4));
    cout << "Enqueued P1, P2, P3\n";
    cout << "Dequeue: P" << queue_dequeue(q).id << "\n";
    cout << "Peek: P" << queue_peek(q).id << "\n";
    cout << "Size: " << queue_get_size(q) << "\n";
    queue_destroy(q);
    
    // Priority Queue Demo
    cout << "\n--- Priority Queue Demo (Min-Heap by burst time) ---\n";
    PriorityQueue* pq = pq_create(compare_by_burst_time);
    pq_insert(pq, create_process(1, 0, 5));
    pq_insert(pq, create_process(2, 0, 2));
    pq_insert(pq, create_process(3, 0, 8));
    cout << "Inserted P1(burst=5), P2(burst=2), P3(burst=8)\n";
    cout << "Extract min: P" << pq_extract_min(pq).id << " (should be P2)\n";
    cout << "Extract min: P" << pq_extract_min(pq).id << " (should be P1)\n";
    pq_destroy(pq);
    
    // Linked List Demo
    cout << "\n--- Linked List Demo (Doubly Linked) ---\n";
    LinkedList* list = list_create();
    list_insert_back(list, create_process(1, 0, 5));
    list_insert_back(list, create_process(2, 1, 3));
    list_insert_front(list, create_process(3, 2, 4));
    cout << "Insert back P1, P2, then front P3\n";
    cout << "Remove front: P" << list_remove_front(list).id << " (should be P3)\n";
    cout << "Remove back: P" << list_remove_back(list).id << " (should be P2)\n";
    cout << "Size: " << list_get_size(list) << "\n";
    list_destroy(list);
    
    // Stack Demo
    cout << "\n--- Stack Demo (LIFO) ---\n";
    Stack* s = stack_create();
    stack_push(s, create_process(1, 0, 5));
    stack_push(s, create_process(2, 1, 3));
    stack_push(s, create_process(3, 2, 4));
    cout << "Pushed P1, P2, P3\n";
    cout << "Pop: P" << stack_pop(s).id << " (should be P3)\n";
    cout << "Peek: P" << stack_peek(s).id << " (should be P2)\n";
    cout << "Size: " << stack_get_size(s) << "\n";
    stack_destroy(s);
    
    cout << "\n=== Demo Complete ===\n";
}

// ==========================================
// Main Function
// ==========================================
int main(int argc, char* argv[]) {
    
    // Check for JSON mode first (no banner output)
    bool has_json = false;
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "--json") { has_json = true; break; }
    }
    
    if (!has_json) {
        cout << "======================================" << endl;
        cout << "   CPU Scheduling Algorithms Simulator" << endl;
        cout << "======================================" << endl;
    }
    
    // Command line mode
    if (argc >= 2) {
        string command = argv[1];
        
        if (command == "--help" || command == "-h") {
            cout << "\nUsage:" << endl;
            cout << "  scheduler                    - Interactive mode" << endl;
            cout << "  scheduler --file <path>      - Load from file" << endl;
            cout << "  scheduler --algo <1-6>       - Run specific algorithm" << endl;
            cout << "  scheduler --quantum <n>      - Set time quantum for RR" << endl;
            cout << "  scheduler --all              - Run all algorithms" << endl;
            cout << "  scheduler --json             - Output results as JSON (for GUI)" << endl;
            cout << "\nAlgorithms:" << endl;
            cout << "  1 - FCFS" << endl;
            cout << "  2 - SJF Non-Preemptive" << endl;
            cout << "  3 - SJF Preemptive (SRTF)" << endl;
            cout << "  4 - Priority Non-Preemptive" << endl;
            cout << "  5 - Priority Preemptive" << endl;
            cout << "  6 - Round Robin" << endl;
            return 0;
        }
        
        string filename = "";
        int algo = 0;
        int quantum = DEFAULT_TIME_QUANTUM;
        bool run_all = false;
        bool json_mode = false;
        
        // JSON mode: --json <algo> <quantum> <count> <a1 b1 p1> <a2 b2 p2> ...
        for (int i = 1; i < argc; i++) {
            string arg = argv[i];
            if (arg == "--json") {
                json_mode = true;
            } else if (arg == "--file" && i + 1 < argc) {
                filename = argv[++i];
            } else if (arg == "--algo" && i + 1 < argc) {
                algo = stoi(argv[++i]);
            } else if (arg == "--quantum" && i + 1 < argc) {
                quantum = stoi(argv[++i]);
            } else if (arg == "--all") {
                run_all = true;
            }
        }
        
        // JSON mode for GUI integration
        if (json_mode) {
            // Read processes from stdin: count, then each line: arrival burst priority
            int count;
            cin >> count;
            api_reset();
            for (int i = 0; i < count; i++) {
                int arrival, burst, priority;
                cin >> arrival >> burst >> priority;
                api_add_process(i + 1, arrival, burst, priority);
            }
            
            if (run_all) {
                cout << "[" << endl;
                for (int a = 1; a <= 6; a++) {
                    if (a == 6) quantum = quantum; // use provided quantum for RR
                    api_run_algorithm(a, quantum);
                    cout << api_get_result_json();
                    if (a < 6) cout << ",";
                }
                cout << "]" << endl;
            } else if (algo >= 1 && algo <= 6) {
                api_run_algorithm(algo, quantum);
                cout << api_get_result_json();
            }
            return 0;
        }
        
        if (!filename.empty()) {
            if (!api_load_from_file(filename.c_str())) {
                cout << "Failed to load file!" << endl;
                return 1;
            }
            print_processes(api_processes, api_process_count);
        }
        
        if (run_all) {
            api_run_all_algorithms(quantum);
        } else if (algo >= 1 && algo <= 6) {
            SchedulingResult* result = api_run_algorithm(algo, quantum);
            if (result) {
                api_print_result();
            }
        }
        
        return 0;
    }
    
    // Interactive mode
    int choice;
    do {
        print_menu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                load_from_file();
                break;
            case 2:
                manual_input();
                break;
            case 3: case 4: case 5: case 6: case 7: case 8:
                run_single_algorithm(choice - 2);  // 3->1, 4->2, etc.
                break;
            case 9:
                if (api_process_count > 0) {
                    int q;
                    cout << "Time Quantum for Round Robin: ";
                    cin >> q;
                    api_run_all_algorithms(q);
                } else {
                    cout << "No processes! Use [1] or [2] first." << endl;
                }
                break;
            case 0:
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 0);
    
    return 0;
}
