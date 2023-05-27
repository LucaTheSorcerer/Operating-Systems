#!/bin/bash

#Schreiben Sie ein Shell-Skript, das eine Textdatei in der Befehlszeile erhält. Das Skript zeigt in der Standardausgabe
#die Anzahl der Leerzeilen und die Anzahl der nicht leeren Zeilen aus der Datei an.

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

#Count the number of empty lines

empty_lines=$(grep -c '^$' $file)
non_empty_lines=$(grep -c -v '^$' $file)

#Add the last line in the file if it is an empty one

if [ -z "$(tail -c 1 $file)" ]; then
    empty_lines=$((empty_lines + 1))
else
    non_empty_lines=$((non_empty_lines + 1))
fi

echo "The number of empty lines is: $empty_lines"
echo "The number of non-empty lines is: $non_empty_lines"