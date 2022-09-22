# Openseekmap
Dit is een onderzoeksporject waarin goede manieren om benaderend een
string te vinden in een databank onderzocht werden. In dit specifieke geval gaat het om
het vinden van straten bij naam. Als iemand bijvoorbeeld
"vrijheid**s**traat brugge" opzoekt, willen we als resultaat
"Vrijheid**ss**traat Brugge" (met 2 s-en) krijgen en niet een van de 14
andere "Vrijheidstraat"en die niet in Brugge liggen. De volledige opgave is
te vinden in `assignment/README.md`

# compilatie
```
cd src
gcc main.c options_parser.c search.c haversine.c entry.c match.c ranking.c datamap.c linkedlist.c query.c edit_distance.c shift_and.c ../assignment/accents.c ../assignment/utf8.c -g -o ./openseekmap -Wall -Werror -lm -std=c18
echo [query] | ./openseekmap [database]
```
