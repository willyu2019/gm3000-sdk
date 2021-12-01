#!/bin/sh

#/opt/GM3000AdminMgr/script==>xxx

export PATH="/opt/apps/com.longmai.gm3000user/files/bin/:$PATH"
export LD_LIBRARY_PATH="/opt/apps/com.longmai.gm3000user/files/lib/:$LD_LIBRARY_PATH"

cd /opt/apps/com.longmai.gm3000user/files/bin/

if [ -f /opt/apps/com.longmai.gm3000user/files/bin/GM3000User ] ; then
./GM3000User &
fi
