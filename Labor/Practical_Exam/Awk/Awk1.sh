#!/bin/bash

#Zeigen Sie für jede eingegebene Datei das Verhältnis zwischen der Anzahl der Selbstlaute (Vokale) und der Anzahl der
#Mitlaute (Konsonanten) an. Die Dateien werden in der Befehlszeile eingegeben. Das Ergebnis wird in der
#Standardausgabe im folgenden Format angezeigt (eine Zeile für jede Datei):
#“Dateiname Anzahl_Vokale Anzahl_Konsonanten”
#Beispiel für die Standardausgabe:
#Datei1 3 5
#Datei2 35 17

# Loop through each file passed as command-line arguments
for file in "$@"; do
    # Use awk to count the number of vowels and consonants in the file
    result=$(awk '
        BEGIN {
            # Initialize vowel and consonant counters
            vowels = 0
            consonants = 0
        }
        {
            # Convert each line to lowercase for easier matching
            line = tolower($0)
            # Loop through each character in the line
            for (i = 1; i <= length(line); i++) {
                # Check if the character is a vowel or consonant
                if (substr(line, i, 1) ~ /[aeiouäöü]/) {
                    vowels++
                } else if (substr(line, i, 1) ~ /[bcdfghjklmnpqrstvwxyzß]/) {
                    consonants++
                }
            }
        }
        END {
            # Print the filename, vowel count, and consonant count
            printf("%s %d %d\n", FILENAME, vowels, consonants)
        }
    ' "$file")
    
    # Print the result to standard output
    echo "$result"
done
