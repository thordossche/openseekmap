#include <string.h>
#include <stdio.h>
#include "linkedlist.h"
#include "match.h"
#include "ranking.h"

struct linked_list* new_match_list() {
    return new_linked_list();
}

void copy_match(void* entry_ptr, void* location) {
    struct Match* match = (struct Match*)entry_ptr;
    struct Match* copy = (struct Match*)location;
    
    copy->size = match->size;
    copy->score = match->score;
    copy->entries = (struct Entry**)malloc(match->size*sizeof(struct Entry*));
    memcpy(copy->entries, match->entries, match->size*sizeof(struct Entry*));
    copy->distances = (int*)malloc(match->size*sizeof(int));
    memcpy(copy->distances, match->distances, match->size*sizeof(int));

}

void free_match(void* entry_ptr) {
    struct Match* match = (struct Match*)entry_ptr;
    free(match->entries);
    free(match->distances);
    free(match);
}

void add_match(struct linked_list* list, struct Entry* entries[], int distances[], size_t size) {
    struct Match match;
    match.size = size;
    match.score = -1;
    match.entries = entries;
    match.distances = distances;
    add_item(list, &match, sizeof(struct Match), &copy_match);
}

void add_match_struct(struct linked_list* list, struct Match* match) {
    add_item(list, &match, sizeof(struct Match), &copy_match);
}

void add_matches_from_list(struct linked_list* list,  struct linked_list* matches) {
    struct list_item* current = matches->start;
    while (current != NULL) {
        add_item(list, current->data, sizeof(struct Match), &copy_match);
        current = current->next;
    }
}
void delete_match(struct linked_list* list, struct Match* match) {
    delete_item(list, match, &free_match);
}

void delete_match_list(struct linked_list* list) {
    delete_list(list, &free_match);
}

void empty_match_list(struct linked_list* list) {
    while (list->start != NULL) {
        delete_item(list, list->start->data, &free_match);
    }
}

void print_match(struct Match* match) {
    struct Entry* highest_rank = NULL;
    int highest_rank_val = -1;
    for (int i = 0; i < match->size; i++) {
        if (match->entries[i]->rank > highest_rank_val) {
            highest_rank = match->entries[i];
            highest_rank_val = match->entries[i]->rank;
        }
    }
    printf("%s (%lu)\n", highest_rank->name, highest_rank->id);
    printf("geo:%f,%f\n", highest_rank->lat, highest_rank->lon);
}

struct Match* create_container(size_t size) {
    struct Match* join = (struct Match*)malloc(sizeof(struct Match));

    join->size = size;

    // join both arrays for entries
    join->entries = (struct Entry**)malloc(join->size*sizeof(struct Entry*));
    
    // join both arrays for distances
    join->distances = malloc(join->size*sizeof(int));
    
   return join; 
}

void join_match_with_container(struct Match* container, struct Match* match, int position) {
    // add the entries
    for (int i = position; i < position + match->size; i++) {
        container->entries[i] = match->entries[i-position];
    }
    
    // add the distances
    for (int i = position; i < position + match->size; i++) {
        container->distances[i] = match->distances[i-position];
    }

}
