#!/bin/bash
#Zeigen Sie den Inhalt für jede eingegebene Datei an. Die Dateien werden in der Befehlszeile eingegeben. Jede Datei wird
#in umgekehrter Reihenfolge der Zeilen angezeigt (zuerst wird die letzte Zeile angezeigt, dann die vorletzte usw.). Das
#Ergebnis wird in der Standardausgabe ohne weitere Zeichen zwischen den Inhalten der Dateien angezeigt.



#Check if the correct number of arguments is given

if [ $# -lt 1 ]; then
    echo "Usage: $0 <file1> <file2> ..."
    exit 1
fi

#Check if the given files exist

for file in $@; do
    if [ ! -f $file ]; then
        echo "File $file does not exist"
        exit 2
    fi
done

for file in "$@"; do
    awk '{a[NR]=$0} END{for(i=NR;i>=1;i--) print a[i]}' "$file"
done