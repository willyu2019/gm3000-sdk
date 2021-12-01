#!/bin/sh


DIR=/opt/mpluginv4/lib
EXEDIR=/opt/mpluginv4/bin
EXE=mPluginV4

export LD_LIBRARY_PATH=$DIR:LD_LIBRARY_PATH

cd $EXEDIR

sleep 2

if [ -f $EXEDIR/$EXE ]; then
	nohup ./$EXE >/dev/null 2>&1 &
	nohup ./$EXE  https 51245 >/dev/null 2>&1 &
fi 