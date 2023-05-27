#Zeigen Sie für jede eingegebene Datei das Verhältnis zwischen der Anzahl der Selbstlaute (Vokale) und der Anzahl der
#Mitlaute (Konsonanten) an. Die Dateien werden in der Befehlszeile eingegeben. Das Ergebnis wird in der Standardausgabe im folgenden Format angezeigt 
#(eine Zeile für jede Datei):
#“Dateiname Anzahl_Vokale Anzahl_Konsonanten”

#!/bin/bash

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
    vocals=$(awk '{print gsub (/[aeiou]/, "")}' "$file")
    consonants=$(awk '{print gsub (/[bcdfghjklmnpqrstvwxyz]/, "")}' "$file")
    echo "$file $vocals $consonants"
done

