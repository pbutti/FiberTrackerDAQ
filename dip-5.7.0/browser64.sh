#!/bin/bash

if [ -z $DIPBASE ]; then
	echo DIPBASE not set, please source the DIP environment - execute "source setenv64.sh"
    exit 1
fi

echo Starting DIP Browser v5.7.0
java -Djava.library.path=$DIPBASE/lib64 -jar tools/dip-browser.jar &

