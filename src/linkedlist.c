#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

// create new linkedlist
struct linked_list* new_linked_list() {
    struct linked_list* list;
    list = (struct linked_list*)malloc(sizeof(struct linked_list));    
    list->start = NULL;
    list->end = NULL;
    list->number_of_items = 0;
    return list;
}

// add item to linkedlist
void add_item(struct linked_list* list, void* data, size_t data_size, void (*copy_data)(void*, void*)) {
    // create list item
    struct list_item* item = (struct list_item*)malloc(sizeof(struct list_item));
    item->next = NULL;
    item->data = malloc(data_size);
    copy_data(data, item->data);
    

    // add item
    if (list->start == NULL) {
        list->start = item;    
        list->end = item;
        list->number_of_items++;
    } else {
        // make connection 
        list->end->next = item;
        // update list
        list->end = item; 
        list->number_of_items++;
    }
}

// print a linkedlist
void print_list(struct linked_list* list, void (*print)(void*)) {
    struct list_item* current = list->start;
    if (current != NULL) {
        while(current != list->end){
            print(current->data);
            current = current->next;
        }
        // print last item
        print(current->data);
    } else {
        printf("List is empty\n");
    }
}

struct list_item* get_item(struct linked_list* list, void* data, int (*cmp)(void*, void*)) {
    struct list_item* current = list->start;
    while(current != NULL && !cmp(data, current->data)) {
        current = current->next;
    }
    if (current != NULL) return current->data;
    return NULL;
}

void delete_item(struct linked_list* list, void* item_to_delete, void (*free_data)(void*)){
    struct list_item* prev = NULL;
    struct list_item* current = list->start;
    while (current != NULL && current->data != item_to_delete) {
        prev = current;
        current = current->next; 
    }
    if (current == NULL) {
        printf("WARNING: %p is not in this list", item_to_delete);
        return;
    }
    
    if (list->start == current) {
        // current is start of list
        list->start = current->next;
    }
    if (list->end == current) {
        // current is end of list
        list->end = prev;
    }
    // change prev to point to the next in the list 
    if (prev != NULL) {
        prev->next = current->next;
    }

    free_data(item_to_delete);
    free(current);
    list->number_of_items--;
} 

// delete the full linkedlist
void delete_list(struct linked_list* list, void (*free_data)(void*)) {
    if (list->start != NULL) {
        struct list_item* item;
        struct list_item* next_item;
       
        item = list->start;
        next_item = item->next; 
        while(item != NULL) {
            free_data(item->data);
            free(item);
            item = next_item;
            if (item != NULL) {
                next_item = item->next;
            }
        }
    }
    free(list);
}
