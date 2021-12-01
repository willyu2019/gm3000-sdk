#!/bin/sh

if [ -d ./lm-gm3000user.mips64el ];then	
	chmod 777 lm-gm3000user.mips64el -R
	chmod 755 ./lm-gm3000user.mips64el/DEBIAN/*
	chmod 755 ./lm-gm3000user.mips64el/DEBIAN -R

	if [ $(getconf WORD_BIT) = '32' ] && [ $(getconf LONG_BIT) = '64' ] ; then
		dpkg -b lm-gm3000user.mips64el 
	fi

else
	if [$1 = ""]];then
		echo "请参照错误提示，输入正确的/.../lm-gm3000user 路径，作为当前脚本的参数。"
	fi

fi
