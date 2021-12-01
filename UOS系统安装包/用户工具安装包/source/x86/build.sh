#!/bin/sh
#dem-setuptree

if [ -d ./com.longmai.gm3000user ];then	
	chmod 777 com.longmai.gm3000user -R
	chmod 755 ./com.longmai.gm3000user/DEBIAN/*
	chmod 755 ./com.longmai.gm3000user/DEBIAN -R

	dpkg -b com.longmai.gm3000user 

else
	if [$1 = ""]];then
		echo "请参照错误提示，输入正确的/.../com.longmai.gm3000user 路径，作为当前脚本的参数。"
	fi
fi
