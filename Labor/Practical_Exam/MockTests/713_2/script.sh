#!/bin/bash


#Check if the enought arguments are given

if [ $# -ne 1 ]
then
    echo "Usage: $0 filename"
    exit 1
fi

#Check if the directory exists

if [ ! -d $1 ]
then
    echo "Directory $1 does not exist"
    exit 2
fi

#Search recursively in all sub directories and return the total number of rows from all files
#Use grep -c to count the number of rows from each file and then use awk to sum all the numbers
#type command is used to find only files and not directories
#-exec command is used to execute the command grep -c "" {} \; for each file found
#-c is used to count the number of rows
#"" is used to match all the lines
#{} is used to pass the file name to the command
#\ is used to escape the ; character
# awk '{sum+=$1} END {print sum}' is used to sum all the numbers from the output of the command grep -c "" {} \;

find $1 -type f -exec grep -c "" {} \; | awk '{sum+=$1} END {print sum}'


