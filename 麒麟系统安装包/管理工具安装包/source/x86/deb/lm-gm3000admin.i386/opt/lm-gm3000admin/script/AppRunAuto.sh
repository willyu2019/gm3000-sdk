#!/bin/sh

export PATH="/opt/lm-gm3000admin/bin/:$PATH"
export LD_LIBRARY_PATH="/opt/lm-gm3000admin/lib/:$LD_LIBRARY_PATH"
export QT_PLUGIN_PATH="/opt/lm-gm3000admin/plugins/:$QT_PLUGIN_PATH"

cd /opt/lm-gm3000admin/bin/

if [ -f /opt/lm-gm3000admin/bin/GM3000Admin ] ; then
sleep 3
./GM3000Admin 7 &
fi
