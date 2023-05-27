#!/bin/bash

# Check if the correct number of parameters is provided
if [ $# -lt 2 ]; then
  echo "Usage: $0 <word> <file1> [<file2> ...]"
  exit 1
fi

word="$1"
shift

# Search for the word in each file and display the filenames
file_count=0
for file in "$@"; do
  if grep -q "$word" "$file"; then
    echo "$file"
    ((file_count++))
  fi
done

# Display the number of files containing the word
echo "Number of files containing the word: $file_count"
