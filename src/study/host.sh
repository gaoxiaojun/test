#!/bin/bash

while read ip name aliases
do
  if [ ! -z "$name" ]; then
    # Use echo -en here to suppress ending the line;
    # aliases may still be added
    echo -en "IP is $ip - its name is $name"
    if [ ! -z "$aliases" ]; then
      echo "    Aliases: $aliases"
    else
      # Just echo a blank line
      echo
    fi
  fi
done < /etc/hosts
