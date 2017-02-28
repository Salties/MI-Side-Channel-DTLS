#!/bin/bash

CONTIKI_ROOT=`pwd`/contiki/
CONTIKI_TOOLS=`pwd`/tools/

if [[ `grep 'CONTIKI_ROOT' -c ~/.bashrc` == '0' ]]; then
	echo "Adding CONTIKI_ROOT to bashrc..."
	echo "export CONTIKI_ROOT=${CONTIKI_ROOT}" >> ~/.bashrc;
else
	echo "CONTIKI_ROOT already exists."
fi;

if [[ `grep 'CONTIKI_TOOLS' -c ~/.bashrc` == '0' ]]; then
	echo "Adding CONTIKI_TOOLS to bashrc..."
	echo "export CONTIKI_TOOLS=${CONTIKI_TOOLS}" >> ~/.bashrc;
	echo "PATH+=:${CONTIKI_TOOLS}" >> ~/.bashrc
else
	echo "CONTIKI_TOOLS already exists."
fi;

echo "Setup complete. Please restart the terminal."
