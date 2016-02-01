#!/bin/bash

if [[ $# == 0 ]]; then
	echo "Usage: skotome TIMER [MSG]"
	exit
fi

{
sleep $1
if [[ $2 == "" ]]; then
	notify-send "部落的战士在呼唤你！"
else
	notify-send "$2"
fi
} &
