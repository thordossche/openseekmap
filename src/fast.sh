# gcc main.c -g -o openseekmap -Wall -Werror -std=c18 && echo 'query' | valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./openseekmap ../belgie.data

#gcc main.c options_parser.c search.c entry.c match.c linkedlist.c edit_distance.c shift_and.c -g -o openseekmap -Wall -Werror -std=c18 && cat "$1" | valgrind -s --leak-check=full --show-leak-kinds=all ./openseekmap ../belgie.data

gcc main.c options_parser.c search.c haversine.c entry.c match.c ranking.c query.c datamap.c linkedlist.c edit_distance.c shift_and.c ../assignment/accents.c ../assignment/utf8.c -O3 -o openseekmap -Wall -Werror -lm -std=c18 && cat "$1" | ./openseekmap ../belgie.data
