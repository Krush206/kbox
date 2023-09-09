#!/system/bin/sh
PORT=1026
echo Starting rsync daemon on port $PORT
$KBOX/bin/rsync --daemon --config $KBOX/etc/rsyncd.conf --port $PORT --log-file /dev/null --no-detach 
