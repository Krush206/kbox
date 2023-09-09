#!/system/bin/sh

if [ ! -e $HOME/.rsa_host_key ] ; then
dropbearkey -t rsa -f $HOME/.rsa_host_key;
fi
