#!/bin/sh

export PATH="/opt/lm-gm3000user/bin/:$PATH"
export LD_LIBRARY_PATH="/opt/lm-gm3000user/lib/:$LD_LIBRARY_PATH"
export QT_PLUGIN_PATH="/opt/lm-gm3000user/plugins/:$QT_PLUGIN_PATH"

cd /opt/lm-gm3000user/bin/

if [ -f /opt/lm-gm3000user/bin/GM3000User ] ; then
sleep 3
./GM3000User 7 &
fi
