#!/bin/bash

#Use grep, sed , awk or any other command that you think is necessary to solve the following task. 
#Schreiben Sie ein Shell-Skript, das eine Textdatei in der Befehlszeile erhält. 
#Das Skript zeigt in der Standardausgabe die Anzahl der Zeilen aus der Datei an, die nicht mit den Zeichen A bis G beginnen.

#Check if the user has entered a file name

if [ $# -eq 0 ]; then
    echo "Usage: $0 filename"
    exit 1
fi


filename=$1

count=$(grep -v "^[A-G]" $filename | wc -l)

echo "The number of lines that do not start with A-G is: $count"