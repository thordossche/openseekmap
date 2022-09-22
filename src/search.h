#ifndef SEARCH_H
#define SEARCH_H

#include "linkedlist.h"
#include "entry.h"
#include "query.h"
#include "datamap.h"
#include "options_parser.h"


struct Ranking* search(struct linked_list* list, struct Options* options);
struct Ranking* search_datamap(struct Datamap* database, struct Options* options);

void search_query(struct linked_list* list, char query[], struct linked_list* matches);
void search_query_datamap(struct Datamap* database, char query[], struct linked_list* matches);
void next_combination(int* combination_counter, char** dest, struct Query* query);
void check_for_ranking_candidates(struct Ranking* ranking, struct linked_list** lists, size_t amount);

#endif
