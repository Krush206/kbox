#!/system/bin/sh
LD_PRELOAD=$KBOX/usr/lib/libfakechroot.so FAKECHROOT_BASE=$KBOX FAKECHROOT_EXCLUDE_PATH=/sdcard:/dev:/tmp:/system:/data/data/kevinboone.androidterm exec $KBOX/bin/bash
