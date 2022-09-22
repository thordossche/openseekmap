#ifndef OPTIONS_PARSER_H
#define OPTIONS_PARSER_H

#include "linkedlist.h"
#include "datamap.h"

struct Options {
    double with_location;
    double lat;
    double lon;
    char* database;
    char* query;
};

struct Options* parse_args(int argc, char *argv[]);
int read_query(struct Options* options);
void process_db(struct Options* options, struct linked_list* list);
void process_db_datamap(struct Options* options, struct Datamap* datamap);
void print_options(struct Options* options);
void free_options(struct Options* options);


#endif
