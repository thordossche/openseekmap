#include <string.h>
#include <stdio.h>
#include "search.h"
#include "entry.h"
#include "options_parser.h"
#include "shift_and.h"
#include "edit_distance.h"
#include "linkedlist.h"
#include "match.h"
#include "ranking.h"
#include "query.h"
#include "datamap.h"
#include "../assignment/utf8.h"
#define min(a,b) (a<b?a:b)

struct Ranking* search(struct linked_list* list, struct Options* options) {
    // split query
    struct Query* query = create_query(options->query);

    // create a ranking
    struct Ranking* ranking = new_ranking(options, query);

    // create array for submatches
    struct linked_list** sub_matches = (struct linked_list**)malloc(query->size*sizeof(struct linked_list*));
    for (int i = 0; i < query->size; i++) {
        sub_matches[i] = new_match_list();
    }

    // create array where combinations parts can be stored
    char** current_query = (char**)malloc(query->size*sizeof(char*));
    for (int i = 0; i < query->size; i++) {
        current_query[i] = (char*)malloc(256*sizeof(char));
    }

    // search every combination
    int current_combination = 0;
    while(!(current_combination >> (query->size-1))) {
        // amount of query parts
        int amount_of_query_parts = __builtin_popcount(current_combination) + 1;

        // get next combination
        next_combination(&current_combination, current_query, query);

        // search all parts
        for (int i = 0; i < amount_of_query_parts; i++) {
            // search part and put matches in corresponding list
            search_query(list, current_query[i], sub_matches[i]); 
        }
        // combine the results to one list of total matches
        check_for_ranking_candidates(ranking, sub_matches, amount_of_query_parts);
        // clear used lists
        for (int i = 0; i < amount_of_query_parts; i++) {
            empty_match_list(sub_matches[i]);
        }
    }

    for (int i = 0; i < query->size; i++) {
        free(sub_matches[i]);
        free(current_query[i]);
    }
    free(sub_matches);
    free(current_query);

    free_query(query);
    return ranking;        
}

struct Ranking* search_datamap(struct Datamap* database, struct Options* options) {
    // split query
    struct Query* query = create_query(options->query);

    // create a ranking
    struct Ranking* ranking = new_ranking(options, query);

    // create array for submatches
    struct linked_list** sub_matches = (struct linked_list**)malloc(query->size*sizeof(struct linked_list*));
    for (int i = 0; i < query->size; i++) {
        sub_matches[i] = new_match_list();
    }

    // create array where combinations parts can be stored
    char** current_query = (char**)malloc(query->size*sizeof(char*));
    for (int i = 0; i < query->size; i++) {
        current_query[i] = (char*)malloc(256*sizeof(char));
    }

    // search every combination
    int current_combination = 0;
    while(!(current_combination >> (query->size-1))) {
        // amount of query parts
        int amount_of_query_parts = __builtin_popcount(current_combination) + 1;

        // get next combination
        next_combination(&current_combination, current_query, query);

        // search all parts
        for (int i = 0; i < amount_of_query_parts; i++) {
            // search part and put matches in corresponding list
            search_query_datamap(database, current_query[i], sub_matches[i]); 
        }
        // combine the results to one list of total matches
        check_for_ranking_candidates(ranking, sub_matches, amount_of_query_parts);
        // clear used lists
        for (int i = 0; i < amount_of_query_parts; i++) {
            empty_match_list(sub_matches[i]);
        }
    }

    for (int i = 0; i < query->size; i++) {
        free(sub_matches[i]);
        free(current_query[i]);
    }
    free(sub_matches);
    free(current_query);

    free_query(query);
    return ranking;        
}

void search_query(struct linked_list* list, char query[], struct linked_list* matches) {
    int query_size = u8_strlen(query);
    int distance = 4;
    struct list_item* current = list->start;
    struct Entry* current_entry; 
    if (current != NULL) {
        current_entry = (struct Entry*)current->data;
    };

    // iterate the database
    while (current != NULL) {
        // check if you can use shiftand, and calculate distance if in boundries
        if (current_entry->len < query_size) { 
            distance = edit_distance(current_entry->name, query);
        } else if (shift_and(current_entry->name, query, 6)) {
            distance = edit_distance(current_entry->name, query);
        } 

        // check if the entry is a possible match
        if (distance <= min(3,1+u8_strlen(query)/3)) {
            // add match
            struct Entry* entries[] = { current_entry };
            int distances[] = { distance };
            add_match(matches, entries, distances, 1);
        }

        // reset the distance
        distance = 4;

        // continue to the next entry
        current = current->next;
        if (current != NULL) {
            current_entry = (struct Entry*)current->data;
        }
    }
}

void search_query_datamap(struct Datamap* database, char query[], struct linked_list* matches) {
    int query_size = u8_strlen(query);
    int distance = 4;
    struct list_item* current;
    struct Entry* current_entry;
   
    int begin = (query_size-3)<1?1:(query_size-3);
    int end = (query_size+3)>database->size?database->size-1:(query_size+3);
    for (int i = begin; i <= end; i++) {
        current = database->map[i]->start;
        if (current != NULL) {
            current_entry = (struct Entry*)current->data;
        };
        // iterate the database
        while (current != NULL) {
            // check if you can use shiftand, and calculate distance if in boundries
            if (current_entry->len < query_size) { 
                distance = edit_distance(current_entry->name, query);
            } else if (shift_and(current_entry->name, query, 6)) {
                distance = edit_distance(current_entry->name, query);
            } 

            // check if the entry is a possible match
            if (distance <= min(3,1+u8_strlen(query)/3)) {
                // add match
                struct Entry* entries[] = { current_entry };
                int distances[] = { distance };
                add_match(matches, entries, distances, 1);
            }

            // reset the distance
            distance = 4;

            // continue to the next entry
            current = current->next;
            if (current != NULL) {
                current_entry = (struct Entry*)current->data;
            }
        }
    }
}

void next_combination(int* combination_counter, char** dest, struct Query* query) {
    int combination = *combination_counter;
    // to keep track of the current split
    int current_part = 0;
    // clear dest
    strcpy(dest[current_part], ""); 
    // copy first part
    strcat(dest[current_part], query->query[0]);
    strcat(dest[current_part], " ");
    for (int i = 0; i < query->size - 1; i++){ 
        // if split, go to next part
        if (combination & (1 << i)) {
            // replace trailing ' '
            dest[current_part][strlen(dest[current_part])-1] = '\0';
            // update current_part
            current_part++;
            // clear new part
            strcpy(dest[current_part], ""); 
        } 
        // copy the current string
        strcat(dest[current_part], query->query[i+1]);
        strcat(dest[current_part], " ");
    }

    // replace trailing ' '
    dest[current_part][strlen(dest[current_part])-1] = '\0';
    
    // update the combination_counter
    (*combination_counter)++; 
}


void combine(struct Ranking* ranking, struct Match* container, struct linked_list** lists, size_t amount, int position) {
    // if position is 0 we just started
    if (!position) {
        
        if (amount == 1) {
            // no recursion, just add the only list to the ranking
            struct list_item* current = lists[0]->start;
            for (int i = 0; i < lists[0]->number_of_items; i++) {
                add_to_ranking(ranking, current->data);
                current = current->next;
            }
        } else {
            // we want to join recursively
            // create a container to join in 
            container = create_container(amount);

            // for every item in the first list combine with the other items in the list
            struct list_item* current = lists[0]->start;
            for (int i = 0; i < lists[0]->number_of_items; i++) {
                // put the current item in the container
                join_match_with_container(container, current->data, 0); 
                // combine with the items of other lists
                combine(ranking, container, lists, amount, 1);     
                // next item
                current = current->next;
            }
            // free the container
            free_match(container);
        }
    } else {
        // if amount == position + 1 we are on the end of the recursion
        if (amount == position + 1) {
            struct list_item* current = lists[position]->start;
            for (int i = 0; i < lists[position]->number_of_items; i++) {
                // add the current match to the end of the container
                join_match_with_container(container, current->data, position); 
                // add the filled container to the ranking
                add_to_ranking(ranking, container);
                // next item
                current = current->next;
            }
        } else {
            // for every item in the current list combine with the items of the other list
            struct list_item* current = lists[position]->start;
            for (int i = 0; i < lists[position]->number_of_items; i++) {
                // put the current item in the container
                join_match_with_container(container, current->data, position); 
                // combine with the items of other lists
                combine(ranking, container, lists, amount, position+1);
                // next item
                current = current->next;
            }
        }
    }
}

void check_for_ranking_candidates(struct Ranking* ranking, struct linked_list** lists, size_t amount) {
    // call the recursive method to combine the lists and add the results to the ranking
    combine(ranking, NULL, lists, amount, 0);    
}
