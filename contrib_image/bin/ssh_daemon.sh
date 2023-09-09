#!/system/bin/sh
. $KBOX/etc/kbox_ports.rc
PORT=$SSHD_PORT
ID=`id -u`

echo Starting ssh daemon on port $PORT as user $LOGNAME
$KBOX/bin/dropbear -r $HOME/.rsa_host_key -E -F -p $PORT \
  -A -U $ID -G $ID -N $LOGNAME -C $LOGNAME

