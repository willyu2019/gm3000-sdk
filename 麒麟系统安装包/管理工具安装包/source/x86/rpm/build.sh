#!/bin/sh
#rpmdev-setuptree

if [ -d ~/rpmbuild/SOURCES ];then
	echo "rm -------------------------------"
	rm ~/rpmbuild/SOURCES/lm-gm3000admin -rf
	echo "cp -------------------------------"
	cp lm-gm3000admin ~/rpmbuild/SOURCES -rf
	chmod 777 ~/rpmbuild/SOURCES/lm-gm3000admin -R

	if [ $(getconf WORD_BIT) = '32' ] && [ $(getconf LONG_BIT) = '64' ] ; then
		rpmbuild -bb  lm-gm3000admin-64.specs 
	else
		rpmbuild -bb  lm-gm3000admin.specs 
	fi

	cp ~/rpmbuild/RPMS/*/lm*.rpm .
else
	if [[ $1 = "" ]];then
		echo "请参照错误提示，输入正确的/.../rpmbuild/SOURCES 路径，作为当前脚本的参数。"
	else
		rm  $1/lm-gm3000admin -rf
		cp  lm-gm3000admin $1 -rf
		chmod 777 $1/lm-gm3000admin -R
	fi

	if [ $(getconf WORD_BIT) = '32' ] && [ $(getconf LONG_BIT) = '64' ] ; then
		rpmbuild -bb  lm-gm3000admin-64.specs 
	else
		rpmbuild -bb  lm-gm3000admin.specs 
	fi
		
	RPM="${$1##*/}"
	cp $RPM/RPMS/*/lm*.rpm .
fi
