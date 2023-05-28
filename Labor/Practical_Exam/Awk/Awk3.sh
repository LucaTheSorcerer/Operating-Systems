#!/bin/bash

#Zeigen Sie den Inhalt für jede eingegebene Datei wie folgt an: die erste Datei wird unverändert angezeigt. Für die nächste
#Datei sollen die Wörter für jede Zeile in umgekehrter Reihenfolge geschrieben werden (die Wörter werden durch ':'
#abgetrennt). Die Dateien werden in der Befehlszeile eingegeben. Der Anzeigemodus wird für die folgenden Dateien
#fortgesetzt. Das Ergebnis wird in der Standardausgabe angezeigt.

#Check if the number of arguments is correct

if [ $# -eq 0 ]; then
    echo "Usage: $0 <filename1> <filename2> ... "
    exit 1
fi

first_file=$1

#Print the first file
cat "$first_file"

shift

#Loop through each file

for file in "$@"; do


    awk '{
      for (i = NF; i >= 1; i--) {
        printf "%s", $i
        if (i > 1) {
            printf ":"
        }
    }
    printf "\n"
    }' "$file"
done