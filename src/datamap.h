#ifndef DATAMAP_H
#define DATAMAP_H

#include "linkedlist.h"
#include "entry.h"

struct Datamap {
    size_t size;
    struct linked_list** map;   
};

struct Datamap* new_datamap(size_t size);
void delete_datamap(struct Datamap* datamap);
struct Datamap* database_to_datamap(struct linked_list* database);
void delete_datamap(struct Datamap* datamap);
struct Datamap* rewrite_datamap(struct Datamap* datamap, int with_delete);
void print_datamap(struct Datamap* datamap);
void add_entry_to_datamap(struct Datamap* datamap, struct Entry* entry);

#endif
