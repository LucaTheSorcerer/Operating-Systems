#!/bin/bash

# Check if the correct number of parameters is provided
if [ $# -lt 2 ]; then
  echo "Usage: $0 <file> <word1> [<word2> ...]"
  exit 1
fi

file="$1"
shift

# Check if the file exists
if [ ! -f "$file" ]; then
  echo "Error: File '$file' does not exist."
  exit 1
fi

# Delete occurrences of each word in the file
for word in "$@"; do
  sed -i "s/$word//g" "$file"
done

# No output to stdout as specified

echo "Occurrences of specified words have been deleted from the file."
