#!/bin/bash

#Identifizieren Sie die Zeilen für jede eingegebene Datei, die an aufeinander folgenden Stellen dasselbe Wort enthalten.
#Zeigen Sie für diese Zeilen die Zeilennummer und das betreffende Wort an (in der Datei, zu der es gehört). Die
#Wörter werden durch Leerzeichen getrennt. Die Dateien werden in der Befehlszeile eingegeben. Das Ergebnis wird in der
#Standardausgabe im folgenden Format angezeigt: für jede identifizierte Zeile wird die Zeilennummer und das betreffende
#Wort angezeigt (durch Leerzeichen abgetrennt). Wenn es sich mehrere Wörter in derselben Zeile befinden, werden diese in
#eine neue Zeile angezeigt. Beispiel für die Standardausgabe:
#2 wort1
#2 wort3
#5 wort

#Check if the number of arguments is correct

if [ $# -eq 0 ]; then
    echo "Usage: $0 <filename1> <filename2> ... "
    exit 1
fi

#Loop through each file

# for file in "$@"; do
#     awk '{
#         for (i = 1; i <= NF; i++) {
#             if ($i == prev_word) {
#                 printf "%d %s\n", NR, $i
#             }
#             prev_word = $i
#         }
#     }' "$file"
# done

# Iterate over the input files
for file in "$@"; do
    echo "Processing file: $file"
    echo "---------------------------"

    # Use awk to identify lines with consecutive same words
    awk '{
        for (i = 2; i <= NF; i++) {
            if ($i == $(i-1)) {
                print NR, $i
            }
        }
    }' "$file"

    echo "---------------------------"
done
