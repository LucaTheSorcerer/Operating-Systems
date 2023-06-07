#!/bin/bash

#Write a shell script using any of the commands grep, sed or awk that returns to the standard output the first and last word from every lin

#Check if the enought arguments are given

if [ $# -ne 1 ]
then
    echo "Usage: $0 filename"
    exit 1
fi

#Check if the file exists

if [ ! -f $1 ]
then
    echo "File $1 does not exist"
    exit 2
fi


#Return the first and last word from every line

cat $1 | awk '{print $1, $NF}'