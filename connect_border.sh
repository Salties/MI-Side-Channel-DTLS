#!/bin/bash

if [[ $1 = "stop" ]]; then
	sudo killall tunslip6;
	echo "stop.";
	exit;
fi

TUNSLIP=./contiki/tools/tunslip6

sudo $TUNSLIP -s /dev/ttyUSB2 aaaa::1/64

if [[ $? = "0" ]];
then
	echo "Connected to border router."
else
	echo "Connection to border router failed."
fi
