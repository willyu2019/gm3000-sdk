#!/bin/sh


DIR=/opt/apps/com.longmai.mpluginv4/files/lib
EXEDIR=/opt/apps/com.longmai.mpluginv4/files/bin
EXE=mPluginV4

export LD_LIBRARY_PATH=$DIR:LD_LIBRARY_PATH

cd $EXEDIR

sleep 2

if [ -f $EXEDIR/$EXE ]; then
	nohup ./$EXE >/dev/null 2>&1 &
	nohup ./$EXE  https 51245 >/dev/null 2>&1 &
fi 
