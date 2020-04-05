#!/bin/bash

do_op (){
  res=0
  case $2 in
      "+")
        let res=$1+$3
        ;;
      "-")
        let res=$1-$3
        ;;
      "*")
        let res=$1*$3
        ;;
      "/")
        let res=$1/$3
        ;;
      *)
        echo "operande unknow."
        ;;
  esac

  echo $res
}

# test if $ or $3 isn't a number ?
echo $#
echo $1, $2, $3
if [ $# == 3 ]; then
  do_op $1 $2 $3
else
  echo "Invalid parameters."
fi
