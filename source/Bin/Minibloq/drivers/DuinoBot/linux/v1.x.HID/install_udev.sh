clear
cp 99-robotgroup.rules /etc/udev/rules.d
#/etc/init.d/udev restart
stop udev
start udev

