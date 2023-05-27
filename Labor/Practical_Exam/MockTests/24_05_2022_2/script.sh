#Schreiben Sie ein Shell-Skript, das eine Textdatei in der Befehlszeile erhält. Das Skript zeigt in der Standardausgabe
#die Zeilen aus der Datei an, die Wörter mit 5 bis 7 Vokalen enthalten. Wörter sind durch Leerzeichen abgetrennt und können
#auch Ziffern und Sonderzeichen enthalten.

#!/bin/bash

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

#Print the lines that contain words with 5 to 7 vowels. The words can contain numbers and special characters

grep -E '\b\w*[aeiouAEIOU]\w*(\w*[aeiouAEIOU]\w*){4,6}\b' $file