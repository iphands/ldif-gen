#!/bin/bash

PROG_NAME="ldif-gen"

mkdir bin

if [ 1 -ne 1 ]
then
    echo "32bit debug:"
    gcc -lcrypt -m32  -g -ggdb -Wall -std=gnu99 -o bin/${PROG_NAME}.debug ${PROG_NAME}.c -march=native
    RET=`expr $RET + $?`
    echo -e "Done $RET\n\n\n"

    echo "32bit O3:"
    gcc -lcrypt -m32 -pipe -fomit-frame-pointer -O3 -Wall -std=gnu99 -o bin/${PROG_NAME}.O3 ${PROG_NAME}.c -march=native
    RET=`expr $RET + $?`
    echo -e "Done $RET\n\n\n"

    echo "32bit Os:"
    gcc -lcrypt -m32 -pipe -fomit-frame-pointer -Os -Wall -std=gnu99 -o bin/${PROG_NAME}.Os ${PROG_NAME}.c -march=native#
    RET=`expr $RET + $?`
    echo -e "Done $RET\n\n\n"

    echo "32bit none:"
    gcc -lcrypt -m32 -fomit-frame-pointer -O0 -Wall -std=gnu99 -o bin/${PROG_NAME}.none ${PROG_NAME}.c
    RET=`expr $RET + $?`
    echo -e "Done $RET\n\n\n"
fi


if [ 1 -eq 1 ]
then
    echo "64bit debug:"
    gcc -lcrypt -m64 -pg -Wall -std=gnu99 -o bin/${PROG_NAME}.gp-64 ${PROG_NAME}.c -march=native
    RET=`expr $RET + $?`
    echo -e "Done $RET\n\n\n"

    echo "64bit debug:"
    gcc -lcrypt -m64 -g -ggdb -Wall -std=gnu99 -o bin/${PROG_NAME}.debug-64 ${PROG_NAME}.c -march=native
    RET=`expr $RET + $?`
    echo -e "Done $RET\n\n\n"

    echo "64bit O3:"
    gcc -lcrypt -m64 -pipe -O3 -Wall -std=gnu99 -o bin/${PROG_NAME}.O3-64 ${PROG_NAME}.c -march=native
    RET=`expr $RET + $?`
    echo -e "Done $RET\n\n\n"

    echo "64bit Os:"
    gcc -lcrypt -m64 -pipe -Os -Wall -std=gnu99 -o bin/${PROG_NAME}.Os-64 ${PROG_NAME}.c -march=native
    RET=`expr $RET + $?`
    echo -e "Done $RET\n\n\n"

    echo "64bit none:"
    gcc -lcrypt -m64 -O0 -Wall -std=gnu99 -o bin/${PROG_NAME}.none-64 ${PROG_NAME}.c
    RET=`expr $RET + $?`
    echo -e "Done $RET\n\n\n"
fi

exit $RET
