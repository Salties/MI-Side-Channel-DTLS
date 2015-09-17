#!/bin/bash

COUNT=100
INTERVAL=1
PINGFLAGS=""
TARGET="aaaa::212:4b00:41e:afc6" #cc2538dk #2
#TARGET="aaaa::212:7400:e33:1389" #Telos B


if [ $# -eq 0 ]
then
	LOGFILE=pingload.log
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
		LOGFILE=$1
	fi
fi

echo "Log file: ${LOGFILE}"

touch ${LOGFILE}
ping6 ${TARGET} -D -O $PINGFLAGS -c ${COUNT} -i $INTERVAL | tee -a ${LOGFILE}
