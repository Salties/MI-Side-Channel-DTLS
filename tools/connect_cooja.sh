#!/bin/bash

TUNSLIP=${CONTIKI_ROOT}/tools/tunslip6

sudo -E ${TUNSLIP} -a 127.0.0.1 aaaa::1/64 -t tun1
