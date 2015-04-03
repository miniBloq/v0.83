nmake -a -f makefile.vc GPIB=0 DEBUG=0
nmake -a -f makefile.vc GPIB=0 DEBUG=0 wxinstall
nmake -a -f makefile.vc GPIB=1 DEBUG=0
nmake -a -f makefile.vc GPIB=1 DEBUG=0 wxinstall
nmake -a -f makefile.vc GPIB=0 DEBUG=1
nmake -a -f makefile.vc GPIB=0 DEBUG=1 wxinstall
nmake -a -f makefile.vc GPIB=1 DEBUG=1
nmake -a -f makefile.vc GPIB=1 DEBUG=1 wxinstall

