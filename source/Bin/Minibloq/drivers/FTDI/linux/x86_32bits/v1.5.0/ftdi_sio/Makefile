
# This Makefile has been simplified as much as possible, by putting all
# generic material, independent of this specific directory, into
# ../Rules.make. Read that file for details

# The usb serial headers
INCLUDEUSBSER := $(shell echo "/usr/src/linux-`uname -r`/drivers/usb/serial/")

TOPDIR  := $(shell pwd)
#TOPDIR = .
include $(TOPDIR)/Rules.make

CFLAGS += -I$(INCLUDEUSBSER) -O

OBJS = ftdi.o

all: $(OBJS)

ftdi.o: ftdi_sio.o
	$(LD) -r $^ -o $@

install:
	install -d $(INSTALLDIR)
	install -c $(OBJS) $(INSTALLDIR)

clean:
	rm -f *.o *~ core
