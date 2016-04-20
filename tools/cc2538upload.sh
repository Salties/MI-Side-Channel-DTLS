#!/bin/bash

BIN=$1;
DEVICE=$2;

sudo python ${CONTIKI_ROOT}/tools/cc2538-bsl/cc2538-bsl.py -e -w -v -p ${DEVICE} ${BIN}.bin
