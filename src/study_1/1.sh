#!/bin/bash

a=b
PROG=`basename $0`
echo "PID for $PROG before exec/source/fork: $$"
export a
echo "$PROG: a is $a"
case $1 in
  exec)
    echo "Using exec..."
    exec ./2.sh;;
  source)
    echo "Using source..."
    . ./2.sh;;
  *)
    echo "Using fork by default..."
    ./2.sh;;
esac
echo "PID for $PROG after exec/source/fork: $$"
echo "$PROG: a is $a"
