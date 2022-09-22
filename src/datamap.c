#include <stdio.h>
#include "datamap.h"
#include "entry.h"
#include "linkedlist.h"

struct Datamap* new_datamap(size_t size){
    struct Datamap* datamap = (struct Datamap*)malloc(sizeof(struct Datamap));

    datamap->size = size;
    datamap->map = (struct linked_list**)malloc(size*sizeof(struct linked_list*));
    for (int i = 0; i < size; i++) {
        datamap->map[i] = new_linked_list();
    }
    return datamap;
}

void delete_datamap(struct Datamap* datamap){
    for (int i = 0; i < datamap->size; i++) {
        delete_list(datamap->map[i], &free_entry);
    }
    free(datamap->map);
    free(datamap);
}

void print_datamap(struct Datamap* datamap){
    printf("Datamap of size %i\n", (int)datamap->size);
    for (int i = 0; i < datamap->size; i++) {
        printf("[%i] -> %i\n", i+1, datamap->map[i]->number_of_items);
    }
}

void extend(struct Datamap* datamap, size_t new_size) {
    datamap->map = (struct linked_list**)realloc(datamap->map, new_size*sizeof(struct linked_list*));
    for (int i = datamap->size; i < new_size; i++) {
        datamap->map[i] = new_linked_list();
    }
    datamap->size = new_size;
}

void add_entry_to_datamap(struct Datamap* datamap, struct Entry* entry) {
    if (datamap->size < entry->len) { 
        extend(datamap, entry->len);
    } 
    add_entry(datamap->map[entry->len-1], entry);
}

int sum_items(struct Datamap* datamap) {
    int sum = 0;
    for (int i = 0; i < datamap->size; i++) {
        sum += datamap->map[i]->number_of_items;
    }
    return sum;
}

struct Datamap* database_to_datamap(struct linked_list* database) {
    struct Datamap* datamap = new_datamap(10);
    
    
    struct list_item* current = database->start;
    struct Entry* current_entry = (struct Entry*)current->data;

    // iterate the database
    while (current != NULL) {
        // check if entry size is already in datamap
        if (current_entry->len <= datamap->size) {
            // add it to the datampa
            add_entry_to_datamap(datamap, current_entry);

        } else {
            // extend the datamap to the right size 
            extend(datamap, current_entry->len);
            // add the entry
            add_entry_to_datamap(datamap, current_entry);
        } 

        // continue to the next entry
        current = current->next;
        if (current != NULL) {
            current_entry = (struct Entry*)current->data;
        }
    }
    return datamap;
}

struct Datamap* rewrite_datamap(struct Datamap* datamap, int with_delete) {
    struct Datamap* rewrite = new_datamap(datamap->size);
    
    for (int i = 0; i < datamap->size; i++) {
        struct list_item* current = datamap->map[i]->start;
        if (current != NULL) {
            struct Entry* current_entry = (struct Entry*)current->data;

            // iterate the linkedlist on position i 
            while (current != NULL) {
                // add the item to the rewrite
                add_entry_to_datamap(rewrite, current_entry);
                // continue to the next entry
                current = current->next;
                if (current != NULL) {
                    current_entry = (struct Entry*)current->data;
                }
            }
        }
    }

    if (with_delete) {
        delete_datamap(datamap);
    }
    return rewrite;
}
