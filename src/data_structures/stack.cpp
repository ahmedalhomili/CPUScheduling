/**
 * @file stack.cpp
 * @brief بنية بيانات المكدس (Stack) - LIFO
 * 
 * المكدس يعمل بمبدأ LIFO (Last In First Out)
 * آخر عنصر يدخل هو أول عنصر يخرج
 */

#ifndef STACK_CPP_INCLUDED
#define STACK_CPP_INCLUDED

#include "../types.cpp"

using namespace std;

// ==========================================
// هياكل المكدس
// ==========================================

// عقدة المكدس
struct StackNode {
    Process data;
    StackNode* next;
};

// هيكل المكدس
struct Stack {
    StackNode* top;
    int size;
};

// ==========================================
// دوال المكدس
// ==========================================

// إنشاء مكدس جديد
Stack* stack_create() {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->top = nullptr;
    s->size = 0;
    return s;
}

// تدمير المكدس
void stack_destroy(Stack* s) {
    if (!s) return;
    
    StackNode* current = s->top;
    while (current) {
        StackNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(s);
}

// التحقق من أن المكدس فارغ
bool stack_is_empty(Stack* s) {
    return s->size == 0;
}

// إضافة عنصر (Push)
void stack_push(Stack* s, Process p) {
    StackNode* node = (StackNode*)malloc(sizeof(StackNode));
    node->data = p;
    node->next = s->top;
    s->top = node;
    s->size++;
}

// إزالة عنصر (Pop)
Process stack_pop(Stack* s) {
    if (stack_is_empty(s)) {
        return create_process(-1, 0, 0, 0);
    }
    
    StackNode* temp = s->top;
    Process data = temp->data;
    s->top = s->top->next;
    free(temp);
    s->size--;
    return data;
}

// الاطلاع على أعلى عنصر (Peek)
Process stack_peek(Stack* s) {
    if (stack_is_empty(s)) {
        return create_process(-1, 0, 0, 0);
    }
    return s->top->data;
}

// الحصول على الحجم
int stack_get_size(Stack* s) {
    return s->size;
}

// ==========================================
// مكدس سجلات التنفيذ (للتتبع)
// ==========================================

// عقدة مكدس التنفيذ
struct ExecutionStackNode {
    ExecutionRecord record;
    ExecutionStackNode* next;
};

// هيكل مكدس التنفيذ
struct ExecutionStack {
    ExecutionStackNode* top;
    int size;
};

// إنشاء مكدس تنفيذ
ExecutionStack* exec_stack_create() {
    ExecutionStack* s = (ExecutionStack*)malloc(sizeof(ExecutionStack));
    s->top = nullptr;
    s->size = 0;
    return s;
}

// تدمير مكدس التنفيذ
void exec_stack_destroy(ExecutionStack* s) {
    if (!s) return;
    
    ExecutionStackNode* current = s->top;
    while (current) {
        ExecutionStackNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(s);
}

// إضافة سجل تنفيذ
void exec_stack_push(ExecutionStack* s, ExecutionRecord r) {
    ExecutionStackNode* node = (ExecutionStackNode*)malloc(sizeof(ExecutionStackNode));
    node->record = r;
    node->next = s->top;
    s->top = node;
    s->size++;
}

// إزالة سجل تنفيذ
ExecutionRecord exec_stack_pop(ExecutionStack* s) {
    if (s->size == 0) {
        ExecutionRecord empty = {-1, 0, 0};
        return empty;
    }
    
    ExecutionStackNode* temp = s->top;
    ExecutionRecord record = temp->record;
    s->top = s->top->next;
    free(temp);
    s->size--;
    return record;
}

// طباعة المكدس (للتصحيح)
void stack_print(Stack* s) {
    cout << "Stack [size=" << s->size << "]: TOP -> ";
    StackNode* current = s->top;
    while (current) {
        cout << "P" << current->data.id << " -> ";
        current = current->next;
    }
    cout << "BOTTOM" << endl;
}

#endif // STACK_CPP_INCLUDED
