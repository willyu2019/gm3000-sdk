#!/bin/sh

installPath=/opt/mpluginv4/conf

if [ "$(whoami )" = "root" ];then
	echo " "
else
	echo "Operation not permitted,are you root?"
	exit 1
fi

if [ "$( which yum )" = "" ];then
	echo " "
else
	if [ "$( which certutil )" = "" ];then
		yum install   libnss3-tools 
	fi
fi

if [ "$( which apt-get )" = "" ];then
	echo " "
else
	if [ "$( which certutil )" = "" ];then
		apt-get install  libnss3-tools
	fi
fi


if [ "$( which certutil )" = "" ];then
	echo "系统在线安装 libnss3-tools工具失败，插件服务无法安装信任证书。"
	echo "请手动将[ $installPath ]安装目录下的server.pem证书导入到火狐等浏览器中。"
else
	### For cert8 (legacy - DBM)
	for certDB in $(find /home -name "cert8.db")
	do
	    certdir=$(dirname ${certDB});
	    certutil -A -n "mPluginCA" -t "TCu,TCu,TCu" -i "$installPath/server.pem" -d dbm:${certdir}
	done

	### For cert9 (SQL)
	for certDB in $(find /home -name "cert9.db")
	do
	    certdir=$(dirname ${certDB});
	    certutil -A -n "mPluginCA" -t "TCu,TCu,TCu" -i "$installPath/server.pem" -d sql:${certdir}
	done
fi

exit 0
