#!/bin/bash

# Check if exactly three arguments were passed by the user if not throw message
if [ "$#" -ne 3 ]; then
  echo "Usage: $0 Enter three numbers to run the program: number1 number2 number3"
  exit 1
fi

# Assign the given arguments to x, y respectively z
x=$1
y=$2
z=$3

d=1
i=1

# The condition to check for the gcd and algorithm implementation
while [ "$i" -le "$x" ] && [ "$i" -le "$y" ] && [ "$i" -le "$z" ]; do
  if [ "$((x % i))" -eq 0 ] && [ "$((y % i))" -eq 0 ] && [ "$((z % i))" -eq 0 ]; then
    d="$i"
  fi
  i=$((i + 1))
done

echo "The GCD of the three entered numbers is: $d"

