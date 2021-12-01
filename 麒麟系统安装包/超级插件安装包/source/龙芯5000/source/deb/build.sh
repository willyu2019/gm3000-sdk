#!/bin/sh
#dem-setuptree

if [ -d ./lm-mpluginv4.loongarch64 ];then	
	chmod 777 lm-mpluginv4.loongarch64 -R
	chmod 755 ./lm-mpluginv4.loongarch64/DEBIAN/*
	chmod 755 ./lm-mpluginv4.loongarch64/DEBIAN -R

	if [ $(getconf WORD_BIT) = '32' ] && [ $(getconf LONG_BIT) = '64' ] ; then
		dpkg -b lm-mpluginv4.loongarch64 
	fi
	
else
	if [$1 = ""]];then
		echo "请参照错误提示，输入正确的/.../lm-mpluginv4 路径，作为当前脚本的参数。"
	fi

fi
