#ifndef ENTRY_H
#define ENTRY_H

#include <stdlib.h>
#include <stdint.h>

struct Entry {
    uint64_t id;
    int rank;
    double lat;
    double lon;
    int len;
    char* name;
};

struct Entry line_to_entry(char* line);
void print_entry(void* entry_ptr);
void free_entry(void* entry_ptr);

struct linked_list* new_entry_list();
void add_entry(struct linked_list* list, struct Entry* entry);
void print_entry_list(struct linked_list* list);
void delete_entry(struct linked_list* list, struct Entry* entry);
void delete_entry_list(struct linked_list* list);
// for testing
struct Entry* get_entry(struct linked_list* list, uint64_t id);

#endif
