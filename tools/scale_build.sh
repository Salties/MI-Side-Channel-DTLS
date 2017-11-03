#!/bin/bash

BSP=${SCALE_ROOT}"/lpc1114fn28/build"
USB="/dev/ttyUSB0"
TARGET_MK=${SCALE_ROOT}"/lpc1114fn28/build/lib/target.mk" 

make -f ${TARGET_MK} BSP=${BSP} USB=${USB} PROJECT=${1} clean all
