#!/bin/bash

# Check if the correct number of parameters is provided
if [ $# -lt 2 ]; then
  echo "Usage: $0 <text> <file1> [<file2> ...]"
  exit 1
fi

text="$1"
shift

# Process each file and display the modified content
for file in "$@"; do
  echo "Processing file: $file"
  modified_content=$(sed "/$text/d" "$file")
  echo "$modified_content"
  echo "---------------------"
done
