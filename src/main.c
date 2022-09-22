#include <stdio.h>
#include <stdlib.h>
#include "options_parser.h"
#include "search.h"
#include "match.h"
#include "ranking.h"
#include "datamap.h"

#include <time.h>

int main(int argc, char *argv[]) {
    // parse options
    struct Options* options = (struct Options*)parse_args(argc, argv);

    // process database
    struct Datamap* datamap_db = new_datamap(10);
    process_db_datamap(options, datamap_db);

    // rewrite the database
    datamap_db = rewrite_datamap(datamap_db, 1);
    // signal end preprocessing
    printf("?\n");

    // read query info
    while (read_query(options)) {
        // search query
        struct Ranking* ranking = search_datamap(datamap_db, options);
        for (int i = 0; i < ranking->number_of_items; i++) {
            print_match(ranking->matches[i]);
            printf("~\n");
        }
        free_ranking(ranking);
        printf("?\n");
    }

    // cleanup
    free_options(options);
    delete_datamap(datamap_db);
    return 0;
}

