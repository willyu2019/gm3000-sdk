#!/bin/sh
#dem-setuptree

if [ -d ./lm-mpluginv4.amd64 ];then	
	chmod 777 lm-mpluginv4.amd64 -R
	chmod 755 ./lm-mpluginv4.amd64/DEBIAN/*
	chmod 755 ./lm-mpluginv4.amd64/DEBIAN -R

	if [ -d ./lm-mpluginv4.i386 ];then	
		chmod 777 lm-mpluginv4.i386 -R
		chmod 755 ./lm-mpluginv4.i386/DEBIAN/*
		chmod 755 ./lm-mpluginv4.i386/DEBIAN -R
	fi

	if [ $(getconf WORD_BIT) = '32' ] && [ $(getconf LONG_BIT) = '64' ] ; then
		dpkg -b lm-mpluginv4.amd64 
	else
		dpkg -b lm-mpluginv4.i386
	fi


else
	if [$1 = ""]];then
		echo "请参照错误提示，输入正确的/.../lm-mpluginv4 路径，作为当前脚本的参数。"
	fi

fi
