#!/bin/sh
#rpmdev-setuptree

if [ -d ~/rpmbuild/SOURCES ];then
	echo "rm -------------------------------"
	rm ~/rpmbuild/SOURCES/lm-gm3000user -rf
	echo "cp -------------------------------"
	cp lm-gm3000user ~/rpmbuild/SOURCES -rf
	chmod 777 ~/rpmbuild/SOURCES/lm-gm3000user -R

	if [ $(getconf WORD_BIT) = '32' ] && [ $(getconf LONG_BIT) = '64' ] ; then
		rpmbuild -bb  lm-gm3000user-64.specs 
	else
		rpmbuild -bb  lm-gm3000user.specs 
	fi

	cp ~/rpmbuild/RPMS/*/lm*.rpm .
else
	if [[ $1 = "" ]];then
		echo "请参照错误提示，输入正确的/.../rpmbuild/SOURCES 路径，作为当前脚本的参数。"
	else
		rm  $1/lm-gm3000user -rf
		cp  lm-gm3000user $1 -rf
		chmod 777 $1/lm-gm3000user -R
	fi

	if [ $(getconf WORD_BIT) = '32' ] && [ $(getconf LONG_BIT) = '64' ] ; then
		rpmbuild -bb  lm-gm3000user-64.specs 
	else
		rpmbuild -bb  lm-gm3000user.specs 
	fi
		
	RPM="${$1##*/}"
	cp $RPM/RPMS/*/lm*.rpm .
fi
