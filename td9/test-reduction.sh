#!/bin/bash

EXEC="./reduction"
let "averageSucces = 0"

# Test sommes
for numbThreads in `seq 10 10 100`
do
  for tabSize in `seq 100 100 1000`
  do
    $EXEC $numbThreads $tabSize +
    let "averageSucces += $?"
  done
done

echo "Sommes average : "$averageSucces" % of success"
