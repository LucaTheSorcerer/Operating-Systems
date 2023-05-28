#!/bin/bash

#Use the grep command to solve the following task. 
#Write a shell script that receives as parameters the name of some files in the command line 
#and displays only the names of the shell scripts so the file that end in .sh extension in the standard output. 
#The names of the shell files are sorted (by name) and displayed one by one on one line. 
#A file is considered a shell script if the "file" utility considers it as a shell script. considers it as a shell script.

#Check if any argument are given to the script

if [ $# -eq 0 ]; then
    echo "Usage: $0 file1 file2... "
    exit 1
fi

# Loop through the command line arguments
for file in "$@"; do
    # Check if the file has .sh extension
    if [[ $file == *.sh ]]; then
        # Check if the file starts with a shebang
        if grep -q "^#!/bin/bash" "$file"; then
            # Print the file name
            echo "$file"
        fi
    fi
done | sort | uniq