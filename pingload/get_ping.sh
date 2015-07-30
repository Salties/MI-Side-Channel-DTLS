#!/bin/bash

if [ $# -eq 0 ]
then 
	OUTPUT=pingload.log
else
	OUTPUT=$1
fi

echo "Output file: ${OUTPUT}"

touch ${OUTPUT}
ping6 aaaa::212:4b00:41e:afc6 -D -c 100 -O -i 1 | tee ${OUTPUT}
