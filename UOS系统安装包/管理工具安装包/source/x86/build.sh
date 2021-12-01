#!/bin/sh
#dem-setuptree

if [ -d ./com.longmai.gm3000admin ];then	
	chmod 777 com.longmai.gm3000admin -R
	chmod 755 ./com.longmai.gm3000admin/DEBIAN/*
	chmod 755 ./com.longmai.gm3000admin/DEBIAN -R

	dpkg -b com.longmai.gm3000admin 

else
	if [$1 = ""]];then
		echo "请参照错误提示，输入正确的/.../com.longmai.gm3000admin 路径，作为当前脚本的参数。"
	fi
fi
