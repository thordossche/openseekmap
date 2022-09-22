#ifndef QUERY_H
#define QUERY_H

struct Query {
    int size;
    char** query;
};
struct Query* create_query(char query_string[]);
void free_query(struct Query* query);

#endif
