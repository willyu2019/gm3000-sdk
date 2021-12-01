#!/bin/sh

#/opt/GM3000AdminMgr/script==>xxx

export PATH="/opt/lm-gm3000admin/bin/:$PATH"
export LD_LIBRARY_PATH="/opt/lm-gm3000admin/lib/:$LD_LIBRARY_PATH"
export QT_PLUGIN_PATH="/opt/lm-gm3000admin/plugins/:$QT_PLUGIN_PATH"

cd /opt/lm-gm3000admin/bin/

if [ -f /opt/lm-gm3000admin/bin/GM3000Admin ] ; then
./GM3000Admin &
fi
