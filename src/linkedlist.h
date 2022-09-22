#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>

struct list_item {
    struct list_item* next;
    void* data;
};

struct linked_list {
    struct list_item* start;
    struct list_item* end;
    int number_of_items;
};

struct linked_list* new_linked_list();
void add_item(struct linked_list* list, void* data, size_t data_size, void (*copy_data)(void*, void*));
struct list_item* get_item(struct linked_list* list, void* data, int (*cmp)(void*, void*));
void print_list(struct linked_list* list, void (*print)(void*));
void delete_item(struct linked_list* list, void* item_to_delete, void (*free_data)(void*));
void delete_list(struct linked_list* list, void (*free_data)(void*));
#endif
