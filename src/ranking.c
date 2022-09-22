#include <stdio.h>
#include <string.h>
#include "options_parser.h"
#include "entry.h"
#include "linkedlist.h"
#include "match.h"
#include "ranking.h"
#include "query.h"
#include "haversine.h"
#include "math.h"
#include "../assignment/utf8.h"
#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)

struct Ranking* new_ranking(struct Options* options, struct Query* query) {
    struct Ranking* ranking =  (struct Ranking*)malloc(sizeof(struct Ranking));

    ranking->number_of_items = 0;
    ranking->query = query;
    for (int i = 0; i < 5; i++) {
        ranking->matches[i] = NULL;
    }
    ranking->with_location = options->with_location;
    ranking->lat = options->lat;
    ranking->lon = options->lon;
    return ranking;
}

void free_ranking(struct Ranking* ranking) {
    for (int i = 0; i < ranking->number_of_items; i++) {
        free_match(ranking->matches[i]);
    }
    free(ranking);
}


double correctness(struct Match* match, struct Query* query) {
    // calculate length of query
    int query_len = 0;
    for (int i = 0; i < query->size; i++) {
        query_len += u8_strlen(query->query[i]);

    }
    // add the spaces
    query_len += query->size - match->size;
    
    // calculate total sum of distances
    int total_distance_sum = 0;
    for (int i = 0; i < match->size; i++) {
        total_distance_sum += match->distances[i];
    }

    return ((double)query_len - (double)total_distance_sum)/(double)query_len;
} 

double S(struct Entry* vi, struct Entry* vj) {
    double s = max(extent(vi->rank), extent(vj->rank))/d_geo(vi->lat, vi->lon, vj->lat, vj->lon);
    return min(2, pow(s, 2));
}

double synergy(struct Match* match) {
    if (match->size == 1) return 0; // if size is 1 there are no parts to calculate synergy
    double s = 0;
    double n = (double)match->size;
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            s += S(match->entries[i], match->entries[j]);
        }
    }
    return s/(n*(n-1));
}

double max_rank(struct Match* match) {
    double r = 0;
    for (int i = 0; i < match->size; i++) {
        if((double)match->entries[i]->rank/(double)30 > r) {
            r = (double)match->entries[i]->rank/(double)30;
        }
    }
    return r;
}

double crow_distance(struct Match* match, struct Ranking* ranking) {
    int highest_rank = -1;
    struct Entry* most_specific_entry = NULL;
    for (int i = 0; i < match->size; i++) {
        if (match->entries[i]->rank > highest_rank) {
            highest_rank = match->entries[i]->rank;
            most_specific_entry = match->entries[i];
        }
    }
    double d = log(d_geo(most_specific_entry->lat, most_specific_entry->lon, ranking->lat, ranking->lon)/extent(most_specific_entry->rank));
    return min(1, max(0, 1 - d/log(1000)));
}

void calculate_score(struct Match* match, struct Query* query) {
    // calculate correctness
    double c = correctness(match, query);
    // calculate synergy
    double s = synergy(match);
    // calculate max rank 
    double r = max_rank(match);
    // set score
    match->score = 0.2*c + 0.7*s + 0.1*r;
}

void calculate_score_with_coordinates(struct Match* match, struct Ranking* ranking) {
    // calculate correctness
    double c = correctness(match, ranking->query);
    // calculate synergy
    double s = synergy(match);
    // calculate max rank 
    double r = max_rank(match);
    // distance between coordinates and most specific element
    double d = crow_distance(match, ranking); 
    // set score
    match->score = 0.15*c + 0.55*s + 0.15*r + 0.15*d;
}

void add_to_ranking(struct Ranking* ranking, struct Match* match) {
    // calculate score
    if (ranking->with_location) {
        calculate_score_with_coordinates(match, ranking);    
    } else {
        calculate_score(match, ranking->query);    
    }
    // check if this match has a better score than the last match
    if (ranking->number_of_items < 5 || match->score > ranking->matches[ranking->number_of_items-1]->score) {
        // make a copy of the match
        struct Match* match_cpy = (struct Match*)malloc(sizeof(struct Match));;
        copy_match(match, match_cpy);
        
        struct Match* prev = NULL;
        struct Match* next = NULL;
        int found_place = 0;
        for (int i = 0; i < ranking->number_of_items; i++) {
            // if we already found the position of the match we copy 
            if (found_place) {
                // copy previous item to prev
                prev = next;
                // copy the current item to next for next round
                next = ranking->matches[i];
                // copy the previous item in the current position
                ranking->matches[i] = prev;
            } else if (match->score > ranking->matches[i]->score) {
                // if our match score is better, add it by replacing current item
                next = ranking->matches[i];
                ranking->matches[i] = match_cpy;
                // set bool
                found_place = 1;
                // update the number_of_items if needed
                if (ranking->number_of_items < 5) ranking->number_of_items++;
            }
        }
        // if we did not found a place, this match the worst in the ranking, but there is still place at the end
        if (!found_place) {
            // add the match to the end of the ranking
            ranking->matches[ranking->number_of_items] = match_cpy;
            // update the number_of_items
            ranking->number_of_items++;
        } else {
            // free the item that fell out of the ranking, but check if it was a NULL that was the last item
            if (next != NULL) {
                free_match(next); 
            }
        }
    }
    // else dont add the match
}
