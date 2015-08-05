#!/bin/bash

i="0"

while [ $i -lt 3 ]
do
	./get_ping.sh E20_G2000.log
	i=$[$i+1]
done
