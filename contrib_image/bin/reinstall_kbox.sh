#!/system/bin/sh

$KBOX/bin/busybox gunzip -c -f $KBOX/../lib/libbase.so | $KBOX/bin/busybox tar -xvf - -C $KBOX
