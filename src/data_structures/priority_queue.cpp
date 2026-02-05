/**
 * @file priority_queue.cpp
 * @brief بنية بيانات طابور الأولوية (Min-Heap)
 * 
 * طابور الأولوية مبني على Min-Heap
 * العنصر ذو الأولوية الأعلى (القيمة الأصغر) يخرج أولاً
 */

#ifndef PRIORITY_QUEUE_CPP_INCLUDED
#define PRIORITY_QUEUE_CPP_INCLUDED

#include "../types.cpp"

using namespace std;

// ==========================================
// نوع دالة المقارنة
// ==========================================
typedef bool (*CompareFunction)(const Process&, const Process&);

// ==========================================
// دوال المقارنة المختلفة
// ==========================================

// مقارنة حسب وقت التنفيذ (للـ SJF)
bool compare_by_burst_time(const Process& a, const Process& b) {
    if (a.burst_time == b.burst_time)
        return a.arrival_time < b.arrival_time;
    return a.burst_time < b.burst_time;
}

// مقارنة حسب الوقت المتبقي (للـ SRTF)
bool compare_by_remaining_time(const Process& a, const Process& b) {
    if (a.remaining_time == b.remaining_time)
        return a.arrival_time < b.arrival_time;
    return a.remaining_time < b.remaining_time;
}

// مقارنة حسب الأولوية
bool compare_by_priority(const Process& a, const Process& b) {
    if (a.priority == b.priority)
        return a.arrival_time < b.arrival_time;
    return a.priority < b.priority;
}

// مقارنة حسب وقت الوصول
bool compare_by_arrival_time(const Process& a, const Process& b) {
    return a.arrival_time < b.arrival_time;
}

// ==========================================
// هيكل طابور الأولوية
// ==========================================
struct PriorityQueue {
    Process* heap;
    int capacity;
    int size;
    CompareFunction compare;
};

// ==========================================
// دوال مساعدة داخلية
// ==========================================

// تبديل عنصرين
void pq_swap(Process& a, Process& b) {
    Process temp = a;
    a = b;
    b = temp;
}

// إعادة ترتيب للأعلى (بعد الإضافة)
void pq_heapify_up(PriorityQueue* pq, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (pq->compare(pq->heap[index], pq->heap[parent])) {
            pq_swap(pq->heap[index], pq->heap[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

// إعادة ترتيب للأسفل (بعد الحذف)
void pq_heapify_down(PriorityQueue* pq, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    
    if (left < pq->size && pq->compare(pq->heap[left], pq->heap[smallest])) {
        smallest = left;
    }
    if (right < pq->size && pq->compare(pq->heap[right], pq->heap[smallest])) {
        smallest = right;
    }
    if (smallest != index) {
        pq_swap(pq->heap[index], pq->heap[smallest]);
        pq_heapify_down(pq, smallest);
    }
}

// ==========================================
// دوال طابور الأولوية
// ==========================================

// إنشاء طابور أولوية جديد
PriorityQueue* pq_create(CompareFunction cmp, int capacity = MAX_PROCESSES) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->heap = (Process*)malloc(sizeof(Process) * capacity);
    pq->capacity = capacity;
    pq->size = 0;
    pq->compare = cmp;
    return pq;
}

// تدمير طابور الأولوية
void pq_destroy(PriorityQueue* pq) {
    if (!pq) return;
    free(pq->heap);
    free(pq);
}

// التحقق من أن الطابور فارغ
bool pq_is_empty(PriorityQueue* pq) {
    return pq->size == 0;
}

// إدخال عنصر
void pq_insert(PriorityQueue* pq, Process p) {
    if (pq->size >= pq->capacity) return;
    pq->heap[pq->size] = p;
    pq_heapify_up(pq, pq->size);
    pq->size++;
}

// استخراج العنصر ذو الأولوية القصوى
Process pq_extract_min(PriorityQueue* pq) {
    if (pq_is_empty(pq)) {
        return create_process(-1, 0, 0, 0);
    }
    Process min = pq->heap[0];
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;
    pq_heapify_down(pq, 0);
    return min;
}

// الاطلاع على أعلى عنصر
Process pq_peek(PriorityQueue* pq) {
    if (pq_is_empty(pq)) {
        return create_process(-1, 0, 0, 0);
    }
    return pq->heap[0];
}

// الحصول على الحجم
int pq_get_size(PriorityQueue* pq) {
    return pq->size;
}

// إعادة بناء الـ heap بالكامل
void pq_rebuild(PriorityQueue* pq) {
    for (int i = (pq->size / 2) - 1; i >= 0; i--) {
        pq_heapify_down(pq, i);
    }
}

#endif // PRIORITY_QUEUE_CPP_INCLUDED
