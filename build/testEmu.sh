#!/bin/bash

./makeFresh.sh

../bash_scripts/prepEmulatorSDimage.sh \
-i /home/owen/bin/CSpect/tbblue.mmc \
-p ../build/bin/ \
-n program.nex \
-d /

mono "/home/owen/bin/CSpect/CSpect.exe" -w3 -com="/dev/ttyUSB0:115200" -zxnext -nextrom -basickeys -exit -tv -brk -emu -mmc=/home/owen/bin/CSpect/tbblue.mmc