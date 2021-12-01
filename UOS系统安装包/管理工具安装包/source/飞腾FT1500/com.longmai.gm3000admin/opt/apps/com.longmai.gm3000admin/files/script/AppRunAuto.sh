#!/bin/sh

export PATH="/opt/apps/com.longmai.gm3000admin/files/bin/:$PATH"
export LD_LIBRARY_PATH="/opt/apps/com.longmai.gm3000admin/files/lib/:$LD_LIBRARY_PATH"

cd /opt/apps/com.longmai.gm3000admin/files/bin/

if [ -f /opt/apps/com.longmai.gm3000admin/files/bin/GM3000Admin ] ; then
sleep 3
./GM3000Admin 7 &
fi
