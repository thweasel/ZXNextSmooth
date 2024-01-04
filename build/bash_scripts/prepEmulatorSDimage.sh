#!/bin/bash

printf "\n = EMULATOR SD CARD PREP = \n"

#Check for hdfmonkey by checking for the "help" message when there are no parameters
if test "$(hdfmonkey)" != "Type 'hdfmonkey help' for usage."; then
    printf "\n [!] hdfmonkey not installed\n\n"
    exit
fi

sddestpath="/"
programpath="./"

while getopts :i:p:n:d:h flag
do
    case "${flag}" in
        i) sdimage=${OPTARG};;
        p) programpath=${OPTARG};;
        n) programname=${OPTARG};;
        d) sddestpath=${OPTARG};;
        h)  printf "\n -i [full path to SDImage] "
            printf "\n -p [program path]"
            printf "\n -n [program name]"
            printf "\n -d [destination path in SDImage]"
            printf "\n\n"
            exit
        ;;
        :) printf "\n [!] Missing parameter on -$OPTARG\n\n"; exit;;
        \?) printf "\n [!] Invalid option -$OPTARG\n\n"; exit;;
    esac
done



if test ! -f "$sdimage"; then
    printf "\n [!] No SD image found\n\n"
    exit
fi

if test ! -f "$programpath$programname"; then
    printf "\n [!] No program found\n\n"
    exit
fi

printf "\n\tEmulator SDImage (full path): $sdimage";
printf "\n\tProgram Path: $programpath";
printf "\n\tProgram Name: $programname";
printf "\n\tDestination in SDImage: $sddestpath";
printf "\n"

printf "\n  Removing $sddestpath$programname from $sdimage"
printf "\n\t>> hdfmonkey rm $sdimage $sddestpath$programname"
printf "\n\t>> $(hdfmonkey rm $sdimage $sddestpath$program)"

printf "\n"

printf "\n  Adding $sddestpath$programname from $sdimage"
printf "\n\t>> hdfmonkey put $sdimage $programpath$programname  $sddestpath$programname"
printf "\n\t>> $(hdfmonkey put $sdimage $programpath$programname $sddestpath$program)"


printf "\n\n === SUCCESS === \n\n"

#hdfmonkey rm /home/owen/bin/CSpect/tbblue.mmc a.nex
#hdfmonkey rm /home/owen/bin/CSpect/tbblue.mmc a.tap

# Lib NEW > NEX file
#zcc +zxn ../LogicTest.c -v -clib=new -create-app -subtype=nex -startup=0 #-Cz"--clean" #--main-fence 0xC000 -startup=31


# Put built files on to CSpect SD-CARD

#hdfmonkey put /home/owen/bin/CSpect/tbblue.mmc a.nex /a.nex
#hdfmonkey put /home/owen/bin/CSpect/tbblue.mmc a.tap /a.tap

#mono "/home/owen/bin/CSpect/CSpect.exe" -w3 -zxnext -nextrom -basickeys -exit -tv -brk -emu -mmc=/home/owen/bin/CSpect/tbblue.mmc