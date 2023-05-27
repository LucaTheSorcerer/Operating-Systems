#!/bin/bash

# Check if the correct number of parameters is provided so 2

if [ $# -lt 2 ]; then
    echo "Usage: $0 <word> <file1> "
    exit 1
fi

#Use the sed command to add a word before each lowercase letter in the file and show the result on the screen

word="$1"
filename="$2"

sed -E "s/([a-z])/$word\1/g" "$filename"
