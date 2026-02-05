/**
 * @file queue.cpp
 * @brief بنية بيانات الطابور (Queue) - FIFO
 * 
 * الطابور يعمل بمبدأ FIFO (First In First Out)
 * أول عنصر يدخل هو أول عنصر يخرج
 */

#ifndef QUEUE_CPP_INCLUDED
#define QUEUE_CPP_INCLUDED

#include "../types.cpp"

using namespace std;

// ==========================================
// هياكل الطابور
// ==========================================

// عقدة الطابور
struct QueueNode {
    Process data;
    QueueNode* next;
};

// هيكل الطابور
struct Queue {
    QueueNode* front;
    QueueNode* rear;
    int size;
};

// ==========================================
// دوال الطابور
// ==========================================

// إنشاء طابور جديد
Queue* queue_create() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = nullptr;
    q->rear = nullptr;
    q->size = 0;
    return q;
}

// تدمير الطابور وتحرير الذاكرة
void queue_destroy(Queue* q) {
    if (!q) return;
    
    QueueNode* current = q->front;
    while (current) {
        QueueNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(q);
}

// التحقق من أن الطابور فارغ
bool queue_is_empty(Queue* q) {
    return q->size == 0;
}

// إضافة عنصر للطابور (من الخلف)
void queue_enqueue(Queue* q, Process p) {
    QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
    node->data = p;
    node->next = nullptr;
    
    if (queue_is_empty(q)) {
        q->front = node;
        q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
    q->size++;
}

// إزالة عنصر من الطابور (من الأمام)
Process queue_dequeue(Queue* q) {
    if (queue_is_empty(q)) {
        return create_process(-1, 0, 0, 0);
    }
    
    QueueNode* temp = q->front;
    Process data = temp->data;
    q->front = q->front->next;
    
    if (!q->front) {
        q->rear = nullptr;
    }
    
    free(temp);
    q->size--;
    return data;
}

// الاطلاع على أول عنصر بدون إزالة
Process queue_peek(Queue* q) {
    if (queue_is_empty(q)) {
        return create_process(-1, 0, 0, 0);
    }
    return q->front->data;
}

// الحصول على حجم الطابور
int queue_get_size(Queue* q) {
    return q->size;
}

#endif // QUEUE_CPP_INCLUDED
