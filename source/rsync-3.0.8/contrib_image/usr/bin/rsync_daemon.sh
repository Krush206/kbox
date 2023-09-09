#!/system/bin/sh
. $KBOX/etc/kbox_ports.rc
PORT=$RSYNC_PORT
echo Starting rsync daemon on port $PORT
$KBOX/usr/bin/rsync --daemon --config $KBOX/etc/rsyncd.conf --port $PORT --log-file /dev/null --no-detach 
