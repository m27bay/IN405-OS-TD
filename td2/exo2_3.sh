#!/bin/bash

### Exo 13 ###
function compile {
  echo "Complilation..."
  gcc -Wall mystery-code.c -o code
  ./code
}

function debug {
  echo "Debuging..."
  gcc -Wall mystery-code.c -g -o code
  gdb ./code
}

if [ $# -eq 1 ]& [ $1 == "release" ]; then
  compile
elif [ $# -eq 1 ] & [ $1 == "debug" ]; then
  debug
else
  echo "bash: '$1' or '$2': invalid parameters."
fi
### End exo 13 ###

