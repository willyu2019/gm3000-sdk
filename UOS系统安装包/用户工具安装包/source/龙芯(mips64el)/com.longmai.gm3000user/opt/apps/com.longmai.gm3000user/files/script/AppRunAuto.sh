#!/bin/sh

export PATH="/opt/apps/com.longmai.gm3000user/files/bin/:$PATH"
export LD_LIBRARY_PATH="/opt/apps/com.longmai.gm3000user/files/lib/:$LD_LIBRARY_PATH"

cd /opt/apps/com.longmai.gm3000user/files/bin/

if [ -f /opt/apps/com.longmai.gm3000user/files/bin/GM3000User ] ; then
sleep 3
./GM3000User 7 &
fi
