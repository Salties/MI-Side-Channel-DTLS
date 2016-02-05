#!/bin/bash

#Notification time. (Default: 30m)
NotifyTime=$(( 30 * 60 * 1000 )) 

if [[ $# == 0 ]]; then
	echo "Usage: skotome TIMER [MSG]"
	exit
fi

{
sleep $1
if [[ $2 == "" ]]; then
	notify-send "$(date):部落的战士在呼唤着提督！" -t $NotifyTime
else
	notify-send "$2" -t $NotifyTime
fi
} &
echo "Alarm set at $(date)."
