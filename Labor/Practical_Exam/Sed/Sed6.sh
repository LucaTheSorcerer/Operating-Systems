#Write a shell script using the sed command to solve the following task. 
#Ersetzen Sie alle Zeichen, die nicht Großbuchstaben sind, durch ein bestimmtes Zeichen. 
#Das Zeichen wird durch den ersten Parameter und die zu verarbeitende Datei wird durch den zweiten Parameter in der Befehlszeile angegeben. 
#Das Ergebnis wird in der Standardausgabe angezeigt.

# Check if the number of arguments is correct

if [ $# -ne 2 ]; then
    echo "Usage: $0 <character> <filename>"
    exit 1
fi

character=$1
filename=$2

# Replace all non-uppercase characters with the given character

sed -E "s/[^A-Z]/$character/g" "$filename"