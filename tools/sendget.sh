#!/bin/bash

while read -p "Use Ctrl^D to start sending GET:" LINE
do
	echo $LINE;
done

while (( 1 ))
do
	echo GET
	sleep 2
done
