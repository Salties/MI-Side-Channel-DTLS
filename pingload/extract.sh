#!/bin/bash

if [ $# -eq 0 ]
then
	echo "Usage: extract LOGFILE"
	exit
fi

cat $1 | gawk '
BEGIN{print "---BEGIN---"}
{
if($1 !~ /rtt|---|PING/ && $2 != "packets" && $3 ~ /bytes|answer/)
	{
		time=$1;
		ping=$8;
		if($time!="" && ping=="") ping="time=0";
		#print time"\t"ping
		print ping
	}
}
END{print "---END---"}' |\
sed -r 's/\[//g; s/\]//g; s/time=//g;'
