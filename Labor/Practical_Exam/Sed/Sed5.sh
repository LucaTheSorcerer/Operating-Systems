#!/bin/bash

#Write a shell script using the sed command to solve the following task.
#Löschen Sie alle Wörter aus einer Datei, die mindestens eine Ziffer enthalten. 
#Die Datei wird als Parameter in der Befehlszeile angegeben. 
#Das Ergebnis wird in der Standardausgabe angezeigt. 
#Für diese Aufgabe enthalten die Wörter nur Buchstaben und Zahlen und sind durch Leerzeichen abgetrennt.

# Check if the number of arguments is correct

if [ $# -ne 1 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

filename=$1

# Delete all words that contain at least one digit

sed -E "s/[[:alnum:]]*[[:digit:]][[:alnum:]]*//g" "$filename"