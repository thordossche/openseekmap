#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "entry.h"
#include "linkedlist.h"
#include "options_parser.h"
#include "datamap.h"

struct Options* parse_args(int argc, char *argv[]) {
    struct Options* options = (struct Options*)malloc(sizeof(struct Options));
    
    // if coordinates are given
    if (argc == 4) {
        options->with_location = 1;
        options->lon = atof(argv[2]);
        options->lat = atof(argv[3]);
    } else {
        options->with_location = 0;
        options->lon = 0;
        options->lat = 0;
    }

    // database
    options->database = (char*)malloc(strlen(argv[1])+1);
    strcpy(options->database, argv[1]);

    // query
    options->query = NULL;

    return options;
}

int read_query(struct Options* options) {
    if (!options->query) {
        options->query = (char*)malloc(512 * sizeof(char));
    }

    int i = 0;
    int c;
    const char EOL = '\n';

    // check if there is input
    if ( (c = getchar()) == EOF || c == EOL) return 0;
    options->query[i] = c;
    i++;
    
    while ( (c = getchar()) != EOL ) {
        options->query[i] = c;
        i++;
    }
    options->query[i] = '\0';

    return 1;
}

void process_db(struct Options* options, struct linked_list* list) {
    FILE* fp = fopen(options->database, "r");
    if (fp == NULL) {
        printf("Could not open database");
        exit(1);
    }
    char line[512];
    while (fgets(line, 512, fp) != NULL) {
        struct Entry entry = line_to_entry(line);
        add_entry(list, &entry);
    }
    fclose(fp);
}

void process_db_datamap(struct Options* options, struct Datamap* datamap) {
    FILE* fp = fopen(options->database, "r");
    if (fp == NULL) {
        printf("Could not open database");
        exit(1);
    }
    char line[512];
    while (fgets(line, 512, fp) != NULL) {
        struct Entry entry = line_to_entry(line);
        add_entry_to_datamap(datamap, &entry);
    }
    fclose(fp);
}

void free_options(struct Options* options) {
    free(options->database);
    free(options->query);
    free(options);
}

void print_options(struct Options* options) {
    printf("OPTIONS:\n");
    printf("\tCoordinates: %f, %f\n", options->lat, options->lon);
    printf("\tDatabase: '%s'\n", options->database);
    printf("\tQuery: '%s'\n\n", options->query);
}
