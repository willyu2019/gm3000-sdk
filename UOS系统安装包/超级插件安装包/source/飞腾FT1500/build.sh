#!/bin/sh
#dem-setuptree

if [ -d ./com.longmai.mpluginv4 ];then	
	chmod 777 com.longmai.mpluginv4 -R
	chmod 755 ./com.longmai.mpluginv4/DEBIAN/*
	chmod 755 ./com.longmai.mpluginv4/DEBIAN -R

	dpkg -b com.longmai.mpluginv4 

else
	if [$1 = ""]];then
		echo "请参照错误提示，输入正确的com.longmai.mpluginv4 路径，作为当前脚本的参数。"
	fi

fi
