#!/bin/bash

OS=`uname`
KERNEL=`uname -r`
MACH=`uname -m`

PWD=`dirname $0`

cd $PWD

function set_execute_permission {
#find ./ -type f -perm +111 | xargs -n 1 -I {} echo chmod +x {}
#find ./ -type l -perm +111 | xargs -n 1 -I {} echo chmod +x {}
#find ./ -type l -perm +111 -exec ls -l {} \; | awk '{print "ln -sf "$11" "$9}'

#echo set_execute_permission

chmod +x ./miniBloq-amd64
chmod +x ./miniBloq-i386
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-gprof
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-gcc-4.3.5
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-readelf
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-size
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-gcc
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avrdude
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-ld
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-ar
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-ranlib
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-objcopy
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-c++filt
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-gcov
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-as
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-cpp
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-gccbug
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-man
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-g++
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-strings
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-addr2line
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-nm
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-objdump
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-strip
chmod +x ../../lang/avrlinux/i386/v4.3.5/bin/avr-c++
chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/gcc/avr/4.3.5/cc1plus
chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/gcc/avr/4.3.5/collect2
chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/gcc/avr/4.3.5/cc1
chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/gcc/avr/4.3.5/install-tools/fixproto
chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/gcc/avr/4.3.5/install-tools/mkinstalldirs
chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/gcc/avr/4.3.5/install-tools/fix-header

chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-gprof
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-gcc-4.3.5
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-readelf
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-size
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-gcc
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avrdude
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-ld
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-ar
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-ranlib
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-objcopy
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-c++filt
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-gcov
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-as
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-cpp
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-gccbug
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-man
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-g++
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-strings
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-addr2line
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-nm
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-objdump
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-strip
chmod +x ../../lang/avrlinux/amd64/v4.3.5/bin/avr-c++
chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/gcc/avr/4.3.5/cc1plus
chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/gcc/avr/4.3.5/collect2
chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/gcc/avr/4.3.5/cc1
chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/gcc/avr/4.3.5/install-tools/fixproto
chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/gcc/avr/4.3.5/install-tools/mkinstalldirs
chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/gcc/avr/4.3.5/install-tools/fix-header

chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/objdump
chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/ranlib
chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/as
chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/ld
chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/strip
chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/objcopy
chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/ar
chmod +x ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/nm

chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/objdump
chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/ranlib
chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/as
chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/ld
chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/strip
chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/objcopy
chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/ar
chmod +x ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/nm

chmod +x ../../tools/hidUploader/v1.0/hid_bootloader_cli.Linux

ln -sf ../../../bin/avr-objdump ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/objdump
ln -sf ../../../bin/avr-ranlib ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/ranlib
ln -sf ../../../bin/avr-as ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/as
ln -sf ../../../bin/avr-ld ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/ld
ln -sf ../../../bin/avr-strip ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/strip
ln -sf ../../../bin/avr-objdump ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/objdump2
ln -sf ../../../bin/avr-objcopy ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/objcopy
ln -sf ../../../bin/avr-ar ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/ar
ln -sf ../../../bin/avr-nm ../../lang/avrlinux/i386/v4.3.5/lib/avr/bin/nm
ln -sf ../../../bin/avr-objdump ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/objdump
ln -sf ../../../bin/avr-ranlib ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/ranlib
ln -sf ../../../bin/avr-as ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/as
ln -sf ../../../bin/avr-ld ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/ld
ln -sf ../../../bin/avr-strip ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/strip
ln -sf ../../../bin/avr-objcopy ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/objcopy
ln -sf ../../../bin/avr-ar ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/ar
ln -sf ../../../bin/avr-nm ../../lang/avrlinux/amd64/v4.3.5/lib/avr/bin/nm

echo set_execute_permission

}

set_execute_permission

if [ "${OS}" = "Linux" ] ; then
    echo $OS
    echo $KERNEL
    echo $MACH
    if [ "${MACH}" = "x86_64" ] ; then
        export LD_LIBRARY_PATH=$PWD/linuxRuntime/amd64/
        $PWD/Minibloq-amd64
    else
#    if [ "${MACH}" = "i386" ] || [ "${MACH}" = "i486" ] || [ "${MACH}" = "i586" ] || [ "${MACH}" = "i686" ] ; then
        export LD_LIBRARY_PATH=$PWD/linuxRuntime/i386/
        $PWD/Minibloq-i386
    fi
else
    xmessage "Unsupported operating system"
fi


