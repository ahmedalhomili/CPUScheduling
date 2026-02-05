/**
 * @file linked_list.cpp
 * @brief بنية بيانات القائمة المترابطة المزدوجة (Doubly Linked List)
 * 
 * قائمة مترابطة ثنائية الاتجاه
 * كل عقدة تحتوي على مؤشر للعقدة السابقة والتالية
 */

#ifndef LINKED_LIST_CPP_INCLUDED
#define LINKED_LIST_CPP_INCLUDED

#include "../types.cpp"

using namespace std;

// ==========================================
// هياكل القائمة المترابطة
// ==========================================

// عقدة القائمة
struct ListNode {
    Process data;
    ListNode* next;
    ListNode* prev;
};

// هيكل القائمة المترابطة
struct LinkedList {
    ListNode* head;
    ListNode* tail;
    int size;
};

// ==========================================
// دوال القائمة المترابطة
// ==========================================

// إنشاء قائمة جديدة
LinkedList* list_create() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->head = nullptr;
    list->tail = nullptr;
    list->size = 0;
    return list;
}

// تدمير القائمة
void list_destroy(LinkedList* list) {
    if (!list) return;
    
    ListNode* current = list->head;
    while (current) {
        ListNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

// التحقق من أن القائمة فارغة
bool list_is_empty(LinkedList* list) {
    return list->size == 0;
}

// إدراج في البداية
void list_insert_front(LinkedList* list, Process p) {
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->data = p;
    node->prev = nullptr;
    node->next = list->head;
    
    if (list->head) {
        list->head->prev = node;
    }
    list->head = node;
    
    if (!list->tail) {
        list->tail = node;
    }
    list->size++;
}

// إدراج في النهاية
void list_insert_back(LinkedList* list, Process p) {
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->data = p;
    node->next = nullptr;
    node->prev = list->tail;
    
    if (list->tail) {
        list->tail->next = node;
    }
    list->tail = node;
    
    if (!list->head) {
        list->head = node;
    }
    list->size++;
}

// حذف من البداية
Process list_remove_front(LinkedList* list) {
    if (list_is_empty(list)) {
        return create_process(-1, 0, 0, 0);
    }
    
    ListNode* temp = list->head;
    Process data = temp->data;
    list->head = list->head->next;
    
    if (list->head) {
        list->head->prev = nullptr;
    } else {
        list->tail = nullptr;
    }
    
    free(temp);
    list->size--;
    return data;
}

// حذف من النهاية
Process list_remove_back(LinkedList* list) {
    if (list_is_empty(list)) {
        return create_process(-1, 0, 0, 0);
    }
    
    ListNode* temp = list->tail;
    Process data = temp->data;
    list->tail = list->tail->prev;
    
    if (list->tail) {
        list->tail->next = nullptr;
    } else {
        list->head = nullptr;
    }
    
    free(temp);
    list->size--;
    return data;
}

// حذف عقدة محددة
void list_remove_node(LinkedList* list, ListNode* node) {
    if (!node) return;
    
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    
    free(node);
    list->size--;
}

// البحث عن عملية بالمعرف
ListNode* list_find_by_id(LinkedList* list, int id) {
    ListNode* current = list->head;
    while (current) {
        if (current->data.id == id) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// الحصول على الحجم
int list_get_size(LinkedList* list) {
    return list->size;
}

// طباعة القائمة (للتصحيح)
void list_print(LinkedList* list) {
    cout << "LinkedList [size=" << list->size << "]: ";
    ListNode* current = list->head;
    while (current) {
        cout << "P" << current->data.id << " -> ";
        current = current->next;
    }
    cout << "NULL" << endl;
}

#endif // LINKED_LIST_CPP_INCLUDED
