#include <assert.h>
#include <stdio.h>
#include "entry_tests.h"
#include "../src/entry.h"
#include "../src/linkedlist.h"

void run_entry_tests() {
    char e0[] = "23107930	Elisabethlaan	26	4.8385154	50.9883276";

    char e1[] = "40821607	Lodewijk Paretlaan	22	2.9586394	51.2377488";
    char e2[] = "124914382	Baroeneput	22	2.7874002	50.7675341";
    char e3[] = "139750585	Rue de la Justice	26	5.2646184	50.5353702";

    struct Entry entry0 = line_to_entry(e0);

    struct Entry entry1 = line_to_entry(e1);
    struct Entry entry2 = line_to_entry(e2);
    struct Entry entry3 = line_to_entry(e3);

    struct linked_list* list = new_entry_list();
    struct Entry* entry;

    add_entry(list, &entry0);
    assert(list->number_of_items == 1);
    assert(list->start == list->end);

    add_entry(list, &entry1);
    assert(list->number_of_items == 2);
    assert(list->start != list->end);

    add_entry(list, &entry2);
    add_entry(list, &entry3);
    assert(list->number_of_items == 4);

    // get a entry that is in the list
    entry =  get_entry(list, entry0.id);
    assert(entry->id == entry0.id); 

    // get a entry that is not in the list
    entry = get_entry(list, 0);
    assert(entry == NULL); 
    
    // delete a entry in the list
    entry =  get_entry(list, entry1.id);
    delete_entry(list, entry);
    assert(list->number_of_items == 3);
    // check if entry is still in the list
    entry = get_entry(list, entry1.id);
    assert(entry == NULL); 

    // delete the first element in the list
    void* start = list->start;
    entry =  get_entry(list, entry0.id);
    delete_entry(list, entry);
    assert(list->number_of_items == 2);
    assert(start != list->start);

    // delete the last element in the list
    void* end = list->end;
    entry = get_entry(list, entry3.id);
    delete_entry(list, entry);
    assert(list->number_of_items == 1);
    assert(end != list->end);

    // delete the last entry in the list
    entry = get_entry(list, entry2.id);
    delete_entry(list, entry);
    assert(list->number_of_items == 0);
    assert(list->start == NULL);
    assert(list->end == NULL);

    // delete empty list
    delete_entry_list(list);
    
    // make new list and add entries
    list = new_entry_list();
    assert(list->number_of_items == 0);
    assert(list->start == list->end);

    add_entry(list, &entry0);
    add_entry(list, &entry0);
    assert(list->number_of_items == 2);

    // delete a list with entries
    delete_entry_list(list);
}
