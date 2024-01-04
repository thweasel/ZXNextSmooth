#!/bin/bash

./makeFresh.sh

./bash_scripts/prepEmulatorSDimage.sh \
-i /home/owen/bin/CSpect/tbblue.mmc \
-p ./bin/ \
-n program.nex \
-d /

mono "/home/owen/bin/CSpect/CSpect.exe" -w3 -zxnext -nextrom -basickeys -exit -tv -brk -emu -mmc=/home/owen/bin/CSpect/tbblue.mmc