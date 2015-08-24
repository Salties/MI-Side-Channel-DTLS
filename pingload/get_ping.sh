#!/bin/bash

COUNT=50000
INTERVAL=1

if [ $# -eq 0 ]
then

	OUTPUT=pingload.log
else
	if [ $1 == "clean" ]
	then
		read -p "WARNING: ALL LOG FILES WILL BE REMOVE(y/n):" -n 1 -r
		echo
		if [[ $REPLY =~ ^[Yy]$ ]]
		then
			rm *.log
			echo "All log files removed."
		fi
		exit
	else
		OUTPUT=$1
	fi
fi

echo "Output file: ${OUTPUT}"

touch ${OUTPUT}
ping6 aaaa::212:4b00:41e:afc6 -D -c ${COUNT} -i $INTERVAL | tee -a ${OUTPUT}
