#ifndef RANKING_H
#define RANKING_H

#include "linkedlist.h"
#include "query.h"
#include "options_parser.h"

struct Ranking {
    int number_of_items;
    struct Query* query;
    struct Match* matches[5];
    int with_location;
    double lat;
    double lon;
};

struct Ranking* new_ranking(struct Options* options, struct Query* query);
void free_ranking(struct Ranking* ranking);

void add_to_ranking(struct Ranking* ranking, struct Match* match);

#endif
