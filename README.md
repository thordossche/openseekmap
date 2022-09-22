# openseekmap

# compile
```
cd src
gcc main.c options_parser.c search.c haversine.c entry.c match.c ranking.c datamap.c linkedlist.c query.c edit_distance.c shift_and.c ../assignment/accents.c ../assignment/utf8.c -g -o ./openseekmap -Wall -Werror -lm -std=c18
echo [query] | ./openseekmap [database]
```
