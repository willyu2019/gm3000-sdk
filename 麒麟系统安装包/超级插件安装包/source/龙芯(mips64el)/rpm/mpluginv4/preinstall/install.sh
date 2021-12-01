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
	echo "插件服务安装成功。"
	echo "如果使用https访问设备，请手动将安装目录下的server.pem证书导入到火狐浏览器中。"
	echo "或者执行[ $installPath/preinstall ]目录下的脚本 install.sh导入证书。"
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
