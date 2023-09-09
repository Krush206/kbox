#!/system/bin/sh
. $KBOX/etc/kbox_ports.rc
PORT=$TELNET_PORT
echo Starting telnet daemon on port $PORT
$KBOX/bin/utelnetd -p $PORT -l $KBOX/../lib/libinitscript.so

