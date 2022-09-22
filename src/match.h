#ifndef MATCH_H
#define MATCH_H

#include "linkedlist.h"
#include "entry.h"
#include "ranking.h"

struct Match {
    size_t size;
    double score;
    struct Entry** entries;
    int* distances;
};
void free_match(void* entry_ptr);

struct linked_list* new_match_list();
void add_match(struct linked_list* list, struct Entry* entries[], int distances[], size_t size);
void add_match_struct(struct linked_list* list, struct Match* match);
void add_matches_from_list(struct linked_list* list,  struct linked_list* matches);
void copy_match(void* entry_ptr, void* location);
void delete_match(struct linked_list* list, struct Match* match);
void delete_match_list(struct linked_list* list);
void empty_match_list(struct linked_list* list);
void print_match(struct Match* match);

struct Match* create_container(size_t size);
void join_match_with_container(struct Match* container, struct Match* match, int position);

#endif
