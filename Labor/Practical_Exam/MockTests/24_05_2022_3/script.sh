#!/bin/bash

#Schreiben Sie ein Shell-Skript, das eine Textdatei in der Befehlszeile erhält. Das Skript zeigt in der Standardausgabe
#die Zeilen aus der Datei an, die Wörter mit 4 bis 6 Konsonanten enthalten. Wörter sind durch Leerzeichen
#abgetrennt und können auch Ziffern und Sonderzeichen enthalten.

#Check if the correct number of arguments is given to the script

if [ $# -ne 1 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

#Check if the file exists

if [ ! -f $1 ]; then
    echo "File $1 does not exist"
    exit 2
fi

file=$1

awk '{
    for (i=1; i<=NF; i++) {
        word = $i
        gsub(/[aeiouAEIOU]/, "", word)
        if (length(word) >= 4 && length(word) <= 6) {
            print
            break
        }
    }
}' "$file"