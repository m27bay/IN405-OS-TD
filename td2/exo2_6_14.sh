#!/bin/bash

# test if isn't a number ?
if [ $# -eq 1 ]; then
  let res=($1+1)*$1
  let res=$res/2
  echo $res
else
  echo "Invalid parameters."
fi
