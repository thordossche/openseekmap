#include <time.h>
#include <stdio.h>
#include <string.h>
#include "options_parser.h"
#include "edit_distance.h"
#include "linkedlist.h"
#include "entry.h"
#include "datamap.h"
#include "search.h"
#include "ranking.h"

double time_edit_distance(int (*distance_f)(char*, char*), struct linked_list* database, char* query) {
    // timer & info
    double t_total = 0;
    clock_t t = clock();
    int itr = 0;

    // iterate the database
    struct list_item* current = database->start;
    struct Entry* current_entry = (struct Entry*)current->data;

    while (current != NULL) {
        // do the work and time
        t = clock();
        distance_f(query, current_entry->name);
        t_total += clock() - t;

        // update itr
        itr++; 

        // continue to the next entry
        current = current->next;
        if (current != NULL) {
            current_entry = (struct Entry*)current->data;
        }
    }
    //printf("For query: '%s'\n", query);
    //printf("Total time: %f\n", (t_total)/CLOCKS_PER_SEC); 
    //printf("Average time: %f\n", (t_total)/CLOCKS_PER_SEC/((double)itr/(double)10000)); 

    return (t_total)/CLOCKS_PER_SEC/((double)itr/(double)10000);
}

void edit_distance_results() { 
    // options struct to parse database
    struct Options options = {0, 0, 0, "../benelux.data", ""};
    
    // load database
    struct linked_list* database = new_entry_list();
    process_db(&options, database);
    
    // queries
    char* queries[] = {"a", "gent", "de sterre gent", "station gent-sint-pieters gent"};

    // measure with pointerswitching
    for (int i = 0; i < 4; i++) {
        double sum_avg_ptr = 0;
        double sum_avg_matrix = 0;
        printf("QUERY: '%s'\n", queries[i]);
        for (int j = 0; j < 50; j++) {
            sum_avg_ptr += time_edit_distance(&edit_distance, database, queries[i]);
            sum_avg_matrix += time_edit_distance(&edit_distance_with_matrix, database, queries[i]);
        } 
        printf("POINTER: %f\n", sum_avg_ptr/(double)50);
        printf("MATRIX : %f\n", sum_avg_matrix/(double)50);
        printf("CHANGE : %f\n", sum_avg_ptr/sum_avg_matrix*(double)100-100.0); 
        printf("========\n");
        // print for latex
        //printf("%s & %f & %f & %f\n", queries[i], sum_avg_matrix/(double)50, sum_avg_ptr/(double)50, sum_avg_ptr/sum_avg_matrix*(double)100-100.0); 
        //printf("========\n");
    }
}

void read_database_results() {
    // options struct to parse database
    struct Options belgie = {0, 0, 0, "../belgie.data", ""};
    struct Options benelux = {0, 0, 0, "../benelux.data", ""};
   
    struct Options options[] = { belgie, benelux }; 
    char* option_names[] = { "belgie", "benelux" }; 

    // timer & info
    clock_t t;
    double list_t = 0;
    double map_t = 0;
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 20; j++) {

            // process db to list
            t = clock();
            struct linked_list* database_list = new_entry_list(); 
            process_db(&(options[i]), database_list);
            list_t += clock() - t;
            delete_entry_list(database_list);

            // process db to datamap
            t = clock();
            struct Datamap* database_db = new_datamap(10);
            process_db_datamap(&(options[i]), database_db);
            map_t += clock() - t;
            delete_datamap(database_db);
            

        }
        printf("For database: %s\n", option_names[i]);
        printf("List: %f\n", (list_t)/CLOCKS_PER_SEC/20);
        printf("Map: %f\n", (map_t)/CLOCKS_PER_SEC/20);
        printf("===========\n");
        list_t = 0;
        map_t = 0;
    }
}

void rewrite_database_results() {
    // options struct to parse database
    struct Options belgie = {0, 0, 0, "../belgie.data", ""};
    struct Options benelux = {0, 0, 0, "../benelux.data", ""};
   
    struct Options options[] = { belgie, benelux }; 
    char* option_names[] = { "belgie", "benelux" }; 

    // timer & info
    clock_t t;
    double map_t = 0;
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 20; j++) {
            // process db to datamap
            t = clock();
            struct Datamap* database_db = new_datamap(10);
            process_db_datamap(&(options[i]), database_db);
            struct Datamap* rewrite = rewrite_datamap(database_db, 1);
            map_t += clock() - t;
            delete_datamap(rewrite);
        }
        printf("For database: %s\n", option_names[i]);
        printf("Processtime with rewrite: %f\n", (map_t)/CLOCKS_PER_SEC/20);
        printf("===========\n");
        map_t = 0;
    }
}

void search_results() {
    // queries
    char* queries[] = { "kollegelaan kortrijk", 
                        "de sterre gent",
                        "steyneveld",
                        "hoge bosstraat eeklo",
                        "ottignies-louvain-la-neuve"
                      };
    
    // databases
    struct Options options = {0, 0, 0, "../belgie.data", ""};

    // list
    struct linked_list* list = new_entry_list(); 
    process_db(&options, list); 

    // datamap
    struct Datamap* datamap = new_datamap(10);
    process_db_datamap(&(options), datamap);

    // datamap + rewrite
    struct Datamap* rewrite = rewrite_datamap(datamap, 0);
    

    // timers & info
    clock_t t;
    double list_t = 0;
    double datamap_t = 0;
    double rewrite_t = 0;

    struct Ranking* ranking;
    for (int i = 0; i < 5; i++) {
        struct Options options = {0, 0, 0, "../belgie.data", queries[i]};
        for (int j = 0; j < 20; j++) {
            // search with list
            t = clock();
            ranking = search(list, &options);
            list_t += clock() - t;
            free_ranking(ranking);

            // search with datamap
            t = clock();
            ranking = search_datamap(datamap, &options);
            datamap_t += clock() - t;
            free_ranking(ranking);
            
            // search with rewrite
            t = clock();
            ranking = search_datamap(rewrite, &options);
            rewrite_t += clock() - t;
            free_ranking(ranking);
        }
        printf("For: %s\n", queries[i]);
        printf("List: %f\n", list_t/CLOCKS_PER_SEC/20); 
        printf("Datamap: %f\n", datamap_t/CLOCKS_PER_SEC/20);
        printf("Rewrite: %f\n", rewrite_t/CLOCKS_PER_SEC/20);
        printf("===================\n");
        //print for latex
        //printf("%s & %f & %f & %f\\\\ \n", queries[i], list_t/CLOCKS_PER_SEC/20, datamap_t/CLOCKS_PER_SEC/20, rewrite_t/CLOCKS_PER_SEC/20);
        // reset timers
        list_t = 0;
        datamap_t = 0;
        rewrite_t = 0;
    }

    // cleanup
    delete_entry_list(list);
    delete_datamap(datamap);
    delete_datamap(rewrite);
}


void shift_and_results() {
    // queries
    char* queries[] = { "kollegelaan kortrijk", 
                        "de sterre gent",
                        "steyneveld",
                        "hoge bosstraat eeklo",
                        "ottignies-louvain-la-neuve"
                      };
    
    // databases
    struct Options options = {0, 0, 0, "../belgie.data", ""};

    // list
    struct linked_list* list = new_entry_list(); 
    process_db(&options, list); 

    // datamap
    struct Datamap* datamap = new_datamap(10);
    process_db_datamap(&(options), datamap);

    // datamap + rewrite
    struct Datamap* rewrite = rewrite_datamap(datamap, 0);
    

    // timers & info
    clock_t t;
    double t_total = 0;

    struct Ranking* ranking;
    for (int i = 0; i < 5; i++) {
        struct Options options = {0, 0, 0, "../belgie.data", queries[i]};
        for (int j = 0; j < 20; j++) {
            // search
            t = clock();
            ranking = search_datamap(datamap, &options);
            t_total += clock() - t;
            free_ranking(ranking);
        }
        printf("For: %s\n", queries[i]);
        printf("Time: %f\n", t_total/CLOCKS_PER_SEC/20);
        printf("===================\n");

        t_total = 0;
    }

    // cleanup
    delete_entry_list(list);
    delete_datamap(datamap);
    delete_datamap(rewrite);
}

void no_vs_with_rewrite_results() {
    // databases
    struct Options options = {0, 0, 0, "../belgie.data", ""};

    // list to take some queries
    struct linked_list* list = new_entry_list(); 
    process_db(&options, list); 

    

    // timers & info
    int max_amount = 20;
    double results_dm[max_amount+1];
    double results_rw[max_amount+1];


    for(int x = 0; x < 50; x++) {
        printf("X: %i\n", x);
        clock_t t;
        double t_dm = 0;
        double t_rw = 0;

        struct Datamap* datamap;
        struct Datamap* rewrite;

        // for caches switch the order
        if(x%2) {
            // datamap + rewrite
            t = clock();
            struct Datamap* datamap_rw = new_datamap(10);
            datamap_rw = new_datamap(10);
            process_db_datamap(&(options), datamap_rw);
             rewrite = rewrite_datamap(datamap_rw, 1);
            t_rw += clock() - t;

            results_dm[0] += t_dm/CLOCKS_PER_SEC/50;
            results_rw[0] += t_rw/CLOCKS_PER_SEC/50;
            
            // datamap
            t = clock();
             datamap = new_datamap(10);
            process_db_datamap(&(options), datamap);
            t_dm += clock() - t;

        } else {
            // datamap
            t = clock();
             datamap = new_datamap(10);
            process_db_datamap(&(options), datamap);
            t_dm += clock() - t;
            
            // datamap + rewrite
            t = clock();
            struct Datamap* datamap_rw = new_datamap(10);
            datamap_rw = new_datamap(10);
            process_db_datamap(&(options), datamap_rw);
             rewrite = rewrite_datamap(datamap_rw, 1);
            t_rw += clock() - t;

            results_dm[0] += t_dm/CLOCKS_PER_SEC/50;
            results_rw[0] += t_rw/CLOCKS_PER_SEC/50;

        }

        srand(time(NULL));
        int r = rand() % 5000;
        for (int amount = 1; amount <= max_amount; amount++) {
            //printf("AMOUNT: %i -> ", amount);

            struct list_item* current = list->start;
            struct Entry* current_entry = (struct Entry*)current->data;
            int size = 4;
            r = rand() % list->number_of_items;
            while(size > 2) {
                //new query
                for (int j = 0; j < r; j++) {
                    // continue to the next entry
                    current = current->next;
                    if (current == NULL) {
                        current = list->start;
                    }
                    if (current != NULL) {
                        current_entry = (struct Entry*)current->data;
                    }
                }
                int n = strlen(current_entry->name);
                size = 0;
                for (int i = 0; i < n; i++) {
                    if (current_entry->name[i] == ' ') {
                        size++;
                    } 
                }
            }
            options.query = current_entry->name;
            printf("%s\n", options.query);
            
            // search datamap
            struct Ranking* ranking;
            t = clock();
            ranking = search_datamap(datamap, &options);
            t_dm += clock() - t;
            free_ranking(ranking);

            // search rewrite
            t = clock();
            ranking = search_datamap(rewrite, &options);
            t_rw += clock() - t;
            free_ranking(ranking);
                
            results_dm[amount] += t_dm/CLOCKS_PER_SEC/50;
            results_rw[amount] += t_rw/CLOCKS_PER_SEC/50;
        }

        // cleanup
        delete_datamap(datamap);
        delete_datamap(rewrite);
    }
    delete_entry_list(list);

    //print results
    printf("plt.plot([ ");
    for (int i = 0; i < max_amount; i++) {
        printf("%i, ", i);
    }
    printf("%i ], ", max_amount);

    printf("[ ");
    for (int i = 0; i < max_amount; i++) {
        printf("%.3f, ", results_dm[i]);
    }
    printf("%.3f", results_dm[max_amount]);
    printf(" ], 'b')\n");
    
    printf("plt.plot([ ");
    for (int i = 0; i < max_amount; i++) {
        printf("%i, ", i);
    }
    printf("%i ], ", max_amount);

    printf("[ ");
    for (int i = 0; i < max_amount; i++) {
        printf("%.3f, ", results_rw[i]);
    }
    printf("%.3f", results_rw[max_amount]);
    printf(" ], 'r')\n");
}

int main() {
    no_vs_with_rewrite_results();
}
