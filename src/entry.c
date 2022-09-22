#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "entry.h"
#include "linkedlist.h"
#include "../assignment/utf8.h"

struct Entry line_to_entry(char* line) {
    char delim[] = "\t";
    struct Entry entry;

    entry.id = atol(strtok(line, delim));
    entry.name = strtok(NULL, delim);
    entry.rank = atoi(strtok(NULL, delim));
    entry.lon = atof(strtok(NULL, delim));
    entry.lat = atof(strtok(NULL, delim));
    entry.len = u8_strlen(entry.name);
    
    return entry;
}

void print_entry(void* entry_ptr) {
    struct Entry* entry = (struct Entry*)entry_ptr;
    printf("Entry: \n");
    printf("\tid: %lu\n", entry->id);
    printf("\tname: %s\n", entry->name);
    printf("\trank: %i\n", entry->rank);
    printf("\tlat: %f\n", entry->lat);
    printf("\tlon: %f\n", entry->lon);
}

void free_entry(void* entry_ptr) {
    struct Entry* entry = (struct Entry*)entry_ptr;
    free(entry->name);
    free(entry);
}

void copy_entry(void* entry_ptr, void* location) {
    struct Entry* entry = (struct Entry*)entry_ptr;
    struct Entry* copy = (struct Entry*)location;
    
    copy->id = entry->id;
    copy->rank = entry->rank;
    copy->lat = entry->lat;
    copy->lon = entry->lon;
    copy->len = entry->len;

    copy->name = (char*)malloc((strlen(entry->name) + 1)*sizeof(char));
    strcpy(copy->name, entry->name);
}

int cmp_entry(void* id_ptr, void* entry_ptr) {
    return *(uint64_t*)id_ptr == ((struct Entry*)entry_ptr)->id;
}

struct linked_list* new_entry_list(){
    return new_linked_list();
}

void add_entry(struct linked_list* list, struct Entry* entry){
    add_item(list, entry, sizeof(struct Entry), &copy_entry);
}

void print_entry_list(struct linked_list* list){
    print_list(list, &print_entry);
}

struct Entry* get_entry(struct linked_list* list, uint64_t id) {
    void* ptr = get_item(list, &id, &cmp_entry);
    if(ptr != NULL) return (struct Entry*)ptr;
    return NULL;
}

void delete_entry(struct linked_list* list, struct Entry* entry){
    delete_item(list, entry, &free_entry);
}

void delete_entry_list(struct linked_list* list){
    delete_list(list, &free_entry);
}
