#!/bin/bash

#REQUEST=$1
min="" #1 bytes
midmin="12345678901" #12 bytes
mid="1234567890123456789012" #23 bytes
midmax="1234567890123456789012345678901234" #35 bytes
max="123456789012345678901234567890123456789012345" #46 bytes

REQUEST=${max}


while read -p "Use Ctrl^D to start sending GET:" LINE
do
	echo $LINE;
done

while (( 1 ))
do
	echo $REQUEST
	sleep 2
done
