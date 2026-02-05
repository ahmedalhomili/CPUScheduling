/**
 * @file main.cpp
 * @brief الملف الرئيسي - محاكي جدولة المعالج
 * 
 * @author Ahmed
 * @date 2024
 * 
 * يدعم:
 * - 6 خوارزميات جدولة
 * - إدخال يدوي أو من ملف
 * - طباعة مخطط Gantt
 * - حساب الإحصائيات
 */

#include "src/api.cpp"

using namespace std;

// ==========================================
// دالة الإدخال اليدوي
// ==========================================
void manual_input() {
    api_reset();
    
    int n;
    cout << "عدد العمليات: ";
    cin >> n;
    
    if (n <= 0 || n > MAX_PROCESSES) {
        cout << "عدد غير صالح!" << endl;
        return;
    }
    
    cout << "\nأدخل بيانات كل عملية:" << endl;
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
// دالة تحميل من ملف
// ==========================================
bool load_from_file() {
    string filename;
    cout << "اسم الملف: ";
    cin >> filename;
    
    if (api_load_from_file(filename.c_str())) {
        print_processes(api_processes, api_process_count);
        return true;
    }
    return false;
}

// ==========================================
// تشغيل خوارزمية واحدة
// ==========================================
void run_single_algorithm(int algo) {
    if (api_process_count == 0) {
        cout << "لا توجد عمليات! أدخل عمليات أولاً." << endl;
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
// عرض توضيحي لهياكل البيانات
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
// الدالة الرئيسية
// ==========================================
int main(int argc, char* argv[]) {
    cout << "======================================" << endl;
    cout << "   CPU Scheduling Algorithms Simulator" << endl;
    cout << "======================================" << endl;
    
    // وضع سطر الأوامر
    if (argc >= 2) {
        string command = argv[1];
        
        if (command == "--help" || command == "-h") {
            cout << "\nUsage:" << endl;
            cout << "  scheduler                    - Interactive mode" << endl;
            cout << "  scheduler --file <path>      - Load from file" << endl;
            cout << "  scheduler --algo <1-6>       - Run specific algorithm" << endl;
            cout << "  scheduler --quantum <n>      - Set time quantum for RR" << endl;
            cout << "  scheduler --all              - Run all algorithms" << endl;
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
        
        for (int i = 1; i < argc; i++) {
            string arg = argv[i];
            if (arg == "--file" && i + 1 < argc) {
                filename = argv[++i];
            } else if (arg == "--algo" && i + 1 < argc) {
                algo = stoi(argv[++i]);
            } else if (arg == "--quantum" && i + 1 < argc) {
                quantum = stoi(argv[++i]);
            } else if (arg == "--all") {
                run_all = true;
            }
        }
        
        if (!filename.empty()) {
            if (!api_load_from_file(filename.c_str())) {
                cout << "فشل تحميل الملف!" << endl;
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
    
    // الوضع التفاعلي
    int choice;
    do {
        print_menu();
        cin >> choice;
        
        switch (choice) {
            case 1: case 2: case 3: case 4: case 5: case 6:
                run_single_algorithm(choice);
                break;
            case 7:
                if (api_process_count > 0) {
                    int q;
                    cout << "Time Quantum for Round Robin: ";
                    cin >> q;
                    api_run_all_algorithms(q);
                } else {
                    cout << "لا توجد عمليات!" << endl;
                }
                break;
            case 8:
                load_from_file();
                break;
            case 9:
                manual_input();
                break;
            case 10:
                demo_data_structures();
                break;
            case 0:
                cout << "شكراً لاستخدام البرنامج!" << endl;
                break;
            default:
                cout << "خيار غير صالح!" << endl;
        }
    } while (choice != 0);
    
    return 0;
}
