#!/bin/bash

if [ $# -lt 1 ]; then
  echo "Usage: $0 <filename>"
  exit 1
fi

filename=$1

sed 's/\([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) /\1 \3 /g' "$filename"