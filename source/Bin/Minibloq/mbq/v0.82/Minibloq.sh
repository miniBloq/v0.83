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
chmod +x ../../avrlinux/i386/v1.0/bin/avr-gprof
chmod +x .././avrlinux/i386/v1.0/bin/avr-gcc-4.3.5
chmod +x ../../avrlinux/i386/v1.0/bin/avr-readelf
chmod +x ../../avrlinux/i386/v1.0/bin/avr-size
chmod +x ../../avrlinux/i386/v1.0/bin/avr-gcc
chmod +x ../../avrlinux/i386/v1.0/bin/avrdude
chmod +x ../../avrlinux/i386/v1.0/bin/avr-ld
chmod +x ../../avrlinux/i386/v1.0/bin/avr-ar
chmod +x ../../avrlinux/i386/v1.0/bin/avr-ranlib
chmod +x ../../avrlinux/i386/v1.0/bin/avr-objcopy
chmod +x ../../avrlinux/i386/v1.0/bin/avr-c++filt
chmod +x ../../avrlinux/i386/v1.0/bin/avr-gcov
chmod +x ../../avrlinux/i386/v1.0/bin/avr-as
chmod +x ../../avrlinux/i386/v1.0/bin/avr-cpp
chmod +x ../../avrlinux/i386/v1.0/bin/avr-gccbug
chmod +x ../../avrlinux/i386/v1.0/bin/avr-man
chmod +x ../../avrlinux/i386/v1.0/bin/avr-g++
chmod +x ../../avrlinux/i386/v1.0/bin/avr-strings
chmod +x ../../avrlinux/i386/v1.0/bin/avr-addr2line
chmod +x ../../avrlinux/i386/v1.0/bin/avr-nm
chmod +x ../../avrlinux/i386/v1.0/bin/avr-objdump
chmod +x ../../avrlinux/i386/v1.0/bin/avr-strip
chmod +x ../../avrlinux/i386/v1.0/bin/avr-c++
chmod +x ../../avrlinux/i386/v1.0/lib/gcc/avr/4.3.5/cc1plus
chmod +x ../../avrlinux/i386/v1.0/lib/gcc/avr/4.3.5/collect2
chmod +x ../../avrlinux/i386/v1.0/lib/gcc/avr/4.3.5/cc1
chmod +x ../../avrlinux/i386/v1.0/lib/gcc/avr/4.3.5/install-tools/fixproto
chmod +x ../../avrlinux/i386/v1.0/lib/gcc/avr/4.3.5/install-tools/mkinstalldirs
chmod +x ../../avrlinux/i386/v1.0/lib/gcc/avr/4.3.5/install-tools/fix-header
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-gprof
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-gcc-4.3.5
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-readelf
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-size
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-gcc
chmod +x ../../avrlinux/amd64/v1.0/bin/avrdude
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-ld
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-ar
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-ranlib
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-objcopy
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-c++filt
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-gcov
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-as
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-cpp
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-gccbug
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-man
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-g++
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-strings
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-addr2line
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-nm
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-objdump
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-strip
chmod +x ../../avrlinux/amd64/v1.0/bin/avr-c++
chmod +x ../../avrlinux/amd64/v1.0/lib/gcc/avr/4.3.5/cc1plus
chmod +x ../../avrlinux/amd64/v1.0/lib/gcc/avr/4.3.5/collect2
chmod +x ../../avrlinux/amd64/v1.0/lib/gcc/avr/4.3.5/cc1
chmod +x ../../avrlinux/amd64/v1.0/lib/gcc/avr/4.3.5/install-tools/fixproto
chmod +x ../../avrlinux/amd64/v1.0/lib/gcc/avr/4.3.5/install-tools/mkinstalldirs
chmod +x ../../avrlinux/amd64/v1.0/lib/gcc/avr/4.3.5/install-tools/fix-header

chmod +x ../../avrlinux/i386/v1.0/lib/avr/bin/objdump
chmod +x ../../avrlinux/i386/v1.0/lib/avr/bin/ranlib
chmod +x ../../avrlinux/i386/v1.0/lib/avr/bin/as
chmod +x ../../avrlinux/i386/v1.0/lib/avr/bin/ld
chmod +x ../../avrlinux/i386/v1.0/lib/avr/bin/strip
chmod +x ../../avrlinux/i386/v1.0/lib/avr/bin/objcopy
chmod +x ../../avrlinux/i386/v1.0/lib/avr/bin/ar
chmod +x ../../avrlinux/i386/v1.0/lib/avr/bin/nm
chmod +x ../../avrlinux/amd64/v1.0/lib/avr/bin/objdump
chmod +x ../../avrlinux/amd64/v1.0/lib/avr/bin/ranlib
chmod +x ../../avrlinux/amd64/v1.0/lib/avr/bin/as
chmod +x ../../avrlinux/amd64/v1.0/lib/avr/bin/ld
chmod +x ../../avrlinux/amd64/v1.0/lib/avr/bin/strip
chmod +x ../../avrlinux/amd64/v1.0/lib/avr/bin/objcopy
chmod +x ../../avrlinux/amd64/v1.0/lib/avr/bin/ar
chmod +x ../../avrlinux/amd64/v1.0/lib/avr/bin/nm
chmod +x ../../hid_bootloader_cli/v1.0/hid_bootloader_cli.Linux


ln -sf ../../../bin/avr-objdump ../../avrlinux/i386/v1.0/lib/avr/bin/objdump
ln -sf ../../../bin/avr-ranlib ../../avrlinux/i386/v1.0/lib/avr/bin/ranlib
ln -sf ../../../bin/avr-as ../../avrlinux/i386/v1.0/lib/avr/bin/as
ln -sf ../../../bin/avr-ld ../../avrlinux/i386/v1.0/lib/avr/bin/ld
ln -sf ../../../bin/avr-strip ../../avrlinux/i386/v1.0/lib/avr/bin/strip
ln -sf ../../../bin/avr-objdump ../../avrlinux/i386/v1.0/lib/avr/bin/objdump2
ln -sf ../../../bin/avr-objcopy ../../avrlinux/i386/v1.0/lib/avr/bin/objcopy
ln -sf ../../../bin/avr-ar ../../avrlinux/i386/v1.0/lib/avr/bin/ar
ln -sf ../../../bin/avr-nm ../../avrlinux/i386/v1.0/lib/avr/bin/nm
ln -sf ../../../bin/avr-objdump ../../avrlinux/amd64/v1.0/lib/avr/bin/objdump
ln -sf ../../../bin/avr-ranlib ../../avrlinux/amd64/v1.0/lib/avr/bin/ranlib
ln -sf ../../../bin/avr-as ../../avrlinux/amd64/v1.0/lib/avr/bin/as
ln -sf ../../../bin/avr-ld ../../avrlinux/amd64/v1.0/lib/avr/bin/ld
ln -sf ../../../bin/avr-strip ../../avrlinux/amd64/v1.0/lib/avr/bin/strip
ln -sf ../../../bin/avr-objcopy ../../avrlinux/amd64/v1.0/lib/avr/bin/objcopy
ln -sf ../../../bin/avr-ar ../../avrlinux/amd64/v1.0/lib/avr/bin/ar
ln -sf ../../../bin/avr-nm ../../avrlinux/amd64/v1.0/lib/avr/bin/nm

echo set_execute_permission

}

set_execute_permission

if [ "${OS}" = "Linux" ] ; then
    echo $OS
    echo $KERNEL
    echo $MACH
    if [ "${MACH}" = "x86_64" ] ; then
        export LD_LIBRARY_PATH=$PWD/mbq/v0.82/linuxRuntime/amd64
        $PWD/mbq/v0.82/miniBloq-amd64
    else
#    if [ "${MACH}" = "i386" ] || [ "${MACH}" = "i486" ] || [ "${MACH}" = "i586" ] || [ "${MACH}" = "i686" ] ; then
        export LD_LIBRARY_PATH=$PWD/mbq/v0.82/linuxRuntime/i386
        $PWD/mbq/v0.82/miniBloq-i386
    fi
else
    xmessage "Unsupported operating system"
fi


