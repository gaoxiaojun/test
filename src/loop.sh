#!/usr/bin/env bash

i=1
while [ i < 10 ]; do
    echo $i;
    i = $i + 1;
done
for i in 1, 10; do
    echo $i;
done
