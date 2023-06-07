#!/bin/bash

#Erstellen Sie ein Shell-Skript, das einen Textdateinamen als Befehlszeilenargument bekommt.
#Das Skript sollte die Anzahl der Zeilen in der Datei zählen, die ein bestimmtes Wort oder
#einen bestimmten Ausdruck enthalten, und die Ergebnisse in der Standardausgabe anzeigen.
#Beispiel:
#input:
#"beispiel.txt"
#Hallo, wie geht es Ihnen?
#Mir geht es gut.
#Und Ihnen?
#Mir geht es auch gut, danke.
#Suchen nach: "geht es"
#output:
#Das Wort "geht es" erscheint in 2 Zeilen.d

#Check if the user has entered a file name and a word to search for

if [ $# -ne 2 ]; then
    echo "Usage: $0 filename word"
    exit 1
fi

filename=$1
word=$2

count=$(grep -c "$word" $filename)
echo "The word \'$word\' appears in $count lines."