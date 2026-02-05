# CPU Scheduling Algorithms Simulator

محاكي خوارزميات جدولة المعالج - مشروع تعليمي بلغة C++

## 📋 نظرة عامة

مشروع محاكاة خوارزميات جدولة العمليات في نظام التشغيل.
- مكتوب بلغة **C++** بأسلوب إجرائي (بدون OOP)
- يستخدم `using namespace std`
- ملفات `.cpp` فقط (بدون header files)
- واجهة رسومية بـ Python/Flet

---

## 🔧 الخوارزميات (4 خوارزميات)

| # | الخوارزمية | الوصف | الملف |
|---|------------|-------|-------|
| 1 | **FCFS** | First Come First Serve - أول من يصل أولاً | `fcfs.cpp` |
| 2 | **SJF** | Shortest Job First - أقصر عملية أولاً | `sjf.cpp` |
| 3 | **Priority** | جدولة الأولوية | `priority.cpp` |
| 4 | **Round Robin** | الجدولة الدائرية | `round_robin.cpp` |

### تفاصيل الخوارزميات:

- **FCFS**: غير استباقية - العملية التي تصل أولاً تُنفذ أولاً
- **SJF**: نسختين (Non-Preemptive & Preemptive/SRTF)
- **Priority**: نسختين (Non-Preemptive & Preemptive) - الرقم الأصغر = أولوية أعلى
- **Round Robin**: كل عملية تأخذ شريحة زمنية (Time Quantum)

---

## 📦 بنى البيانات (4 بنى)

| # | البنية | الوصف | الاستخدام |
|---|--------|-------|-----------|
| 1 | **Queue** | طابور FIFO | FCFS, Round Robin |
| 2 | **Priority Queue** | Min-Heap | SJF, Priority |
| 3 | **Linked List** | قائمة مترابطة مزدوجة | تخزين العمليات |
| 4 | **Stack** | مكدس LIFO | تتبع التنفيذ |

---

## 📁 هيكل المشروع

```
CPUScheduling/
├── main.cpp                              # البرنامج الرئيسي
├── CMakeLists.txt                        # ملف البناء
├── README.md                             # هذا الملف
│
├── src/
│   ├── types.cpp                         # الأنواع والثوابت الأساسية
│   ├── api.cpp                           # واجهة برمجية للربط مع Python
│   ├── io_handler.cpp                    # التعامل مع الملفات والطباعة
│   │
│   ├── algorithms/                       # ═══ الخوارزميات ═══
│   │   ├── algorithms.cpp                # ملف تجميع (يتضمن الأربعة)
│   │   ├── fcfs.cpp                      # FCFS
│   │   ├── sjf.cpp                       # SJF (Non-Preemptive + Preemptive)
│   │   ├── priority.cpp                  # Priority (Non-Preemptive + Preemptive)
│   │   └── round_robin.cpp               # Round Robin
│   │
│   └── data_structures/                  # ═══ بنى البيانات ═══
│       ├── queue.cpp                     # طابور FIFO
│       ├── priority_queue.cpp            # طابور أولوية (Min-Heap)
│       ├── linked_list.cpp               # قائمة مترابطة مزدوجة
│       └── stack.cpp                     # مكدس LIFO
│
├── data/                                 # ملفات الإدخال
│   ├── sample_input.txt
│   └── test_*.txt
│
└── gui/                                  # واجهة Python
    ├── scheduler_gui.py                  # واجهة Flet
    └── scheduler_api.py                  # API بايثون
```

---

## 🔗 سلسلة الـ Includes

```
main.cpp
└── src/api.cpp
    └── src/io_handler.cpp
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

**ملاحظة:** كل خوارزمية تستخدم بنية بيانات واحدة:
| الخوارزمية | بنية البيانات |
|------------|---------------|
| FCFS | Queue (طابور) |
| SJF | Priority Queue (طابور أولوية) |
| Priority | Linked List (قائمة مترابطة) |
| Round Robin | Stack (مكدس) + Queue |

---

## 🛠️ البناء والتشغيل

### المتطلبات
- CMake 3.20+
- C++17 compiler (GCC, Clang, MSVC)
- Python 3.8+ (للواجهة الرسومية)
- Flet 0.28+ (للواجهة الرسومية)

### البناء باستخدام CMake
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### البناء المباشر بـ g++
```bash
g++ -std=c++17 -o scheduler main.cpp
```

### التشغيل
```bash
# الوضع التفاعلي
./scheduler

# تحميل من ملف
./scheduler --file data/sample_input.txt

# تشغيل خوارزمية محددة
./scheduler --file data/sample_input.txt --algo 1

# المساعدة
./scheduler --help
```

---

## 📄 صيغة ملف الإدخال

```csv
id,arrival_time,burst_time,priority
1,0,6,2
2,1,8,1
3,2,7,3
4,3,3,4
5,4,4,2
```

---

## 📊 الإحصائيات المحسوبة

- ✅ متوسط وقت الانتظار (Average Waiting Time)
- ✅ متوسط وقت الدوران (Average Turnaround Time)
- ✅ نسبة استخدام المعالج (CPU Utilization)
- ✅ مخطط Gantt

---

## 🎯 الدوال الرئيسية

### الخوارزميات
```cpp
SchedulingResult fcfs(Process[], int count);
SchedulingResult sjf_non_preemptive(Process[], int count);
SchedulingResult sjf_preemptive(Process[], int count);
SchedulingResult priority_non_preemptive(Process[], int count);
SchedulingResult priority_preemptive(Process[], int count);
SchedulingResult round_robin(Process[], int count, int quantum);
```

### بنى البيانات
```cpp
// Queue
Queue* queue_create();
void queue_enqueue(Queue*, Process);
Process queue_dequeue(Queue*);

// Priority Queue
PriorityQueue* pq_create(CompareFunction);
void pq_insert(PriorityQueue*, Process);
Process pq_extract_min(PriorityQueue*);

// Linked List
LinkedList* list_create();
void list_insert_back(LinkedList*, Process);
Process list_remove_front(LinkedList*);

// Stack
Stack* stack_create();
void stack_push(Stack*, Process);
Process stack_pop(Stack*);
```

---

## 🎓 للتعلم

هذا المشروع مصمم لتعلم:
1. بنى البيانات (Queue, Priority Queue, Linked List, Stack)
2. خوارزميات جدولة العمليات
3. برمجة C++ بأسلوب إجرائي
4. ربط C++ مع Python

---

## 📝 License

MIT License
