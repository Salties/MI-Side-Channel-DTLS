#!/bin/bash

device="/dev/ttyUSB0"

if [[ ! -z $1 ]]; then
	device=$1;
fi

echo $1

TUNSLIP=${CONTIKI_ROOT}/tools/tunslip6

sudo $TUNSLIP -s ${device} aaaa::1/64

if [[ $? = "0" ]];
then
	echo "Connected to border router."
else
	echo "Connection to border router failed."
fi
