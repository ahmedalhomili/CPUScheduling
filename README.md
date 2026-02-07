# CPU Scheduling Algorithms Simulator

محاكي خوارزميات جدولة المعالج - مشروع تعليمي بلغة C++

## 📋 نظرة عامة

مشروع محاكاة خوارزميات جدولة العمليات في نظام التشغيل.
- **Backend**: مكتوب بلغة C++ (Procedural - بدون OOP)
- **Frontend**: واجهة رسومية بـ Python/Flet 0.80.5
- **التكامل**: Python تستدعي C++ exe عبر `subprocess` + JSON API
- **بنى البيانات**: مبنية من الصفر بـ `malloc/free` - بدون STL

---

## 🔧 الخوارزميات (6 خوارزميات)

| # | الخوارزمية | النوع | بنية البيانات | الملف |
|---|------------|-------|---------------|-------|
| 1 | **FCFS** | Non-Preemptive | Queue (طابور) | `fcfs.cpp` |
| 2 | **SJF Non-Preemptive** | Non-Preemptive | Priority Queue (Min-Heap) | `sjf.cpp` |
| 3 | **SJF Preemptive (SRTF)** | Preemptive | Priority Queue (Min-Heap) | `sjf.cpp` |
| 4 | **Priority Non-Preemptive** | Non-Preemptive | Linked List (مزدوجة) | `priority.cpp` |
| 5 | **Priority Preemptive** | Preemptive | Linked List (مزدوجة) | `priority.cpp` |
| 6 | **Round Robin** | Preemptive | Queue (طابور) | `round_robin.cpp` |

---

## 📦 بنى البيانات (4 بنى - مبنية من الصفر)

| # | البنية | الوصف | الخوارزمية المستخدمة فيها |
|---|--------|-------|--------------------------|
| 1 | **Queue** | طابور FIFO (`malloc/free`) | FCFS, Round Robin |
| 2 | **Priority Queue** | Min-Heap (مصفوفة ديناميكية) | SJF NP, SRTF |
| 3 | **Linked List** | قائمة مترابطة مزدوجة | Priority NP, Priority P |
| 4 | **Stack** | مكدس LIFO | تتبع سجلات التنفيذ |

---

## 📁 هيكل المشروع

```
CPUScheduling/
├── main.cpp                    # نقطة الدخول + Interactive/CLI/JSON modes
├── sched2.exe                  # الملف التنفيذي (C++ backend)
├── CMakeLists.txt              # ملف البناء CMake
├── requirements.txt            # flet==0.80.5
│
├── src/
│   ├── types.cpp               # الأنواع (Process, SchedulingResult, ExecutionRecord)
│   ├── api.cpp                 # API + JSON output (api_get_result_json)
│   ├── io_handler.cpp          # القوائم والطباعة والملفات
│   │
│   ├── algorithms/
│   │   ├── algorithms.cpp      # ملف تجميع (يتضمن الأربعة)
│   │   ├── fcfs.cpp            # FCFS
│   │   ├── sjf.cpp             # SJF NP + SRTF (Preemptive)
│   │   ├── priority.cpp        # Priority NP + Priority P
│   │   └── round_robin.cpp     # Round Robin
│   │
│   └── data_structures/
│       ├── queue.cpp           # Queue (FIFO) - malloc/free
│       ├── priority_queue.cpp  # Priority Queue (Min-Heap)
│       ├── linked_list.cpp     # Doubly Linked List
│       └── stack.cpp           # Stack (LIFO)
│
├── gui/
│   └── scheduler_gui.py       # واجهة Flet (تستدعي sched2.exe عبر subprocess)
│
├── data/                       # ملفات إدخال تجريبية
│   ├── sample_input.txt
│   ├── test_5_processes.txt
│   ├── test_8_processes.txt
│   └── test_professor_sample.txt
│
└── docs/
    └── PROJECT_DOCUMENTATION.md
```

---

## 🔗 سلسلة الـ Includes

```
main.cpp
└── src/api.cpp                 (JSON API + api_get_result_json)
    └── src/io_handler.cpp      (print_menu, print_gantt_chart, file I/O)
        └── src/algorithms/algorithms.cpp
            │
            ├── fcfs.cpp ──────────► queue.cpp ──────────┐
            │                                            │
            ├── sjf.cpp ───────────► priority_queue.cpp ─┤
            │                                            ├──► types.cpp
            ├── priority.cpp ──────► linked_list.cpp ────┤
            │                                            │
            └── round_robin.cpp ───► stack.cpp ──────────┘
```

---

## 🔌 التكامل: Python ↔ C++

```
Python GUI (Flet)                    C++ Backend (sched2.exe)
┌─────────────┐                     ┌──────────────────────┐
│ scheduler_  │  subprocess.run()   │                      │
│ gui.py      │ ──────────────────► │  sched2.exe          │
│             │  stdin: processes   │  --json --algo N     │
│             │  stdout: JSON       │  --json --all        │
│             │ ◄────────────────── │  --quantum Q         │
└─────────────┘                     └──────────────────────┘
```

**stdin format**: `count\narrival burst priority\n...`
**stdout format**: JSON with algorithm, processes, timeline, statistics

---

## 🛠️ البناء والتشغيل

### المتطلبات
| المتطلب | الإصدار |
|---------|---------|
| C++ Standard | C++17 |
| Compiler | GCC/MinGW (g++) |
| Python | 3.8+ |
| Flet | 0.80.5 |

### البناء بـ g++ (MinGW)
```bash
# إضافة MinGW إلى PATH (إذا لزم الأمر)
# Windows PowerShell:
$env:Path = "F:\Program Files\JetBrains\CLion 2025.2.4\bin\mingw\bin;$env:Path"

# البناء
g++ -std=c++17 -o sched2.exe main.cpp
```

### البناء باستخدام CMake
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### التشغيل

```bash
# الوضع التفاعلي (قائمة تفاعلية)
./sched2.exe

# تحميل من ملف
./sched2.exe --file data/sample_input.txt

# تشغيل خوارزمية محددة
./sched2.exe --file data/sample_input.txt --algo 1

# JSON API (للواجهة الرسومية)
echo "3\n0 7 2\n1 4 1\n2 9 3" | ./sched2.exe --json --algo 1
echo "3\n0 7 2\n1 4 1\n2 9 3" | ./sched2.exe --json --all --quantum 2

# تشغيل الواجهة الرسومية
pip install flet==0.80.5
python gui/scheduler_gui.py
```

### القائمة التفاعلية

```
========== CPU Scheduling Simulator ==========
[1] Load from File
[2] Manual Input
[3] FCFS
[4] SJF Non-Preemptive
[5] SJF Preemptive (SRTF)
[6] Priority Non-Preemptive
[7] Priority Preemptive
[8] Round Robin
[9] Run All Algorithms
[0] Exit
===============================================
```

---

## 📊 الإحصائيات المحسوبة

- ✅ وقت الإكمال (Completion Time) لكل عملية
- ✅ وقت الانتظار (Waiting Time) لكل عملية
- ✅ وقت الدوران (Turnaround Time) لكل عملية
- ✅ متوسط وقت الانتظار (Average Waiting Time)
- ✅ متوسط وقت الدوران (Average Turnaround Time)
- ✅ نسبة استخدام المعالج (CPU Utilization %)
- ✅ مخطط Gantt (مدمج + لكل وحدة زمنية للخوارزميات الاستباقية)

---

## 📄 صيغة ملف الإدخال

```
3
0 7 2
1 4 1
2 9 3
```
> السطر الأول: عدد العمليات. كل سطر بعده: `arrival_time burst_time priority`

---

## 🎯 الدوال الرئيسية

### الخوارزميات
```cpp
SchedulingResult fcfs(Process[], int count);
SchedulingResult sjf_non_preemptive(Process[], int count);
SchedulingResult sjf_preemptive(Process[], int count);          // SRTF
SchedulingResult priority_non_preemptive(Process[], int count);
SchedulingResult priority_preemptive(Process[], int count);
SchedulingResult round_robin(Process[], int count, int quantum);
```

### API (JSON Output)
```cpp
string api_get_result_json();   // Returns JSON for GUI integration
```

### بنى البيانات
```cpp
// Queue (FIFO)
Queue* queue_create();
void queue_enqueue(Queue*, Process);
Process queue_dequeue(Queue*);

// Priority Queue (Min-Heap)
PriorityQueue* pq_create(CompareFunction);
void pq_insert(PriorityQueue*, Process);
Process pq_extract_min(PriorityQueue*);

// Linked List (Doubly)
LinkedList* list_create();
void list_insert_back(LinkedList*, Process);
Process list_remove_front(LinkedList*);

// Stack (LIFO)
Stack* stack_create();
void stack_push(Stack*, Process);
Process stack_pop(Stack*);
```

---

## 🎓 للتعلم

هذا المشروع مصمم لتعلم:
1. **بنى البيانات** - Queue, Priority Queue (Min-Heap), Linked List, Stack - من الصفر بـ malloc/free
2. **خوارزميات جدولة المعالج** - 6 خوارزميات بأنماط مختلفة
3. **برمجة C++ إجرائية** - Procedural Programming بدون OOP أو STL
4. **تكامل اللغات** - ربط C++ مع Python عبر subprocess + JSON

---

## 📝 License

MIT License
