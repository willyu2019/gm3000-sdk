#!/bin/sh
#rpmdev-setuptree

if [ -d ~/rpmbuild/SOURCES ];then
	echo "rm -------------------------------"
	rm ~/rpmbuild/SOURCES/mpluginv4 -rf
	echo "cp -------------------------------"
	cp mpluginv4 ~/rpmbuild/SOURCES -rf
	chmod 777 ~/rpmbuild/SOURCES/mpluginv4 -R

	if [ $(getconf WORD_BIT) = '32' ] && [ $(getconf LONG_BIT) = '64' ] ; then
		rpmbuild -bb  mpluginv4-64.specs 
	fi

	cp ~/rpmbuild/RPMS/*/lm*.rpm .
else
	if [[ $1 = "" ]];then
		echo "请参照错误提示，输入正确的/.../rpmbuild/SOURCES 路径，作为当前脚本的参数。"
	else
		rm  $1/mpluginv4 -rf
		cp  mpluginv4 $1 -rf
		chmod 777 $1/mpluginv4 -R
	fi

	if [ $(getconf WORD_BIT) = '32' ] && [ $(getconf LONG_BIT) = '64' ] ; then
		rpmbuild -bb  mpluginv4-64.specs 
	fi
		
	RPM="${$1##*/}"
	cp $RPM/RPMS/*/lm*.rpm .
fi
