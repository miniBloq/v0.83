OS = OS_LINUX
#OS = OS_MACOSX
#OS = OS_WINDOWS

PROG = main

ifeq ($(OS), OS_LINUX)

TARGET = main
CC = gcc
STRIP = strip
CFLAGS = -Wall -O2 -D$(OS)
LIBS = -lusb
OBJS = $(PROG).o usb_LINUX.o ihex.o

else ifeq ($(OS), OS_MACOSX)

TARGET = $(PROG).dmg
SDK = /Developer/SDKs/MacOSX10.6.sdk
ARCH = -mmacosx-version-min=10.6 -arch x86_64
CC = gcc
STRIP = strip
CFLAGS = -Wall -O2 -D$(OS) -isysroot $(SDK) $(ARCH)
LIBS = $(ARCH) -Wl,-syslibroot,$(SDK) -framework IOKit -framework CoreFoundation
OBJS = $(PROG).o usb_MACOSX.o ihex.o

else ifeq ($(OS), OS_WINDOWS)

TARGET = $(PROG).exe
CC = gcc
STRIP = strip
CFLAGS = -Wall -O2 -D$(OS)
LIBS = -lhid -lsetupapi
OBJS = $(PROG).o usb_WINDOWS.o ihex.o

endif


all: $(TARGET)

ifeq ($(OS), OS_WINDOWS)
$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LIBS)
	$(STRIP) $(TARGET)
else
$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LIBS)
	$(STRIP) $(PROG)
endif

$(PROG).exe: $(PROG)
	cp $(PROG) $(PROG).exe

$(PROG).dmg: $(PROG)
	mkdir tmp
	cp $(PROG) tmp
	hdiutil create -ov -volname "Raw HID Test" -srcfolder tmp $(PROG).dmg
	rm -rf tmp

usb.o: usb_$(OS).c usb_XXX.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(PROG) $(PROG).exe $(PROG).dmg
	rm -rf tmp
