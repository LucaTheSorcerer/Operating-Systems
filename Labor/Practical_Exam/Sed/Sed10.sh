#!/bin/bash

#Use the sed command to solve the following task. 
#Write a shell script that replaces the words from positions 1 and 3 from a file. 
#The file is passed as a parameter on the command line. 
#The words only contain letters and are separated from any other characters. 
#The result will be displayed on the standard output.

if [ $# -lt 1 ]; then
  echo "Usage: $0 <filename>"
  exit 1
fi

filename=$1

# Replace words at positions 1 and 3 in the file
sed -E 's/([[:alpha:]]+)([^[:alpha:]]+)([[:alpha:]]+)([^[:alpha:]]+)([[:alpha:]]+)/\3\2\1\4\5/g' "$filename"