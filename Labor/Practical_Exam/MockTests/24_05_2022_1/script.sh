#!/bin/bash

#Schreiben Sie ein Shell-Skript, das mehrere Textdateien in der Befehlszeile empfängt. Das Skript gibt in der Standardausgabe
#die Anzahl aller nicht leeren Leerzeilen aus allen Dateien zurück (eine einzige Zahl).

#Check if the correct number of arguments is given to the script

if [ $# -lt 1 ]; then
    echo "Usage: $0 <file1> <file2> ..."
    exit 1
fi

#Check if the files exist

for file in $@; do
    if [ ! -f $file ]; then
        echo "File $file does not exist"
        exit 2
    fi
done

#Count the number of non empty lines from all the files

for file in "$@"; do
    non_empty=$(grep -c -v '^$' $file)
    total=$((total + non_empty))
done

echo "The total number of non-empty lines in the files is: $total"