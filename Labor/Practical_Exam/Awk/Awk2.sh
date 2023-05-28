#!/bin/bash

#Zeigen Sie den Inhalt für jede eingegebene Datei an. Die Dateien werden in der Befehlszeile eingegeben. Jede Datei wird
#in umgekehrter Reihenfolge der Zeilen angezeigt (zuerst wird die letzte Zeile angezeigt, dann die vorletzte usw.). Das
#Ergebnis wird in der Standardausgabe ohne weitere Zeichen zwischen den Inhalten der Dateien angezeigt.

#Check if the number of arguments is correct

if [ $# -eq 0 ]; then
    echo "Usage: $0 <filename1> <filename2> ... "
    exit 1
fi

#Loop through each file

for file in "$@"; do
    awk '{ lines[NR] = $0 } END { for (i = NR; i >= 1; i--) print lines[i] }' "$file"
    echo
done