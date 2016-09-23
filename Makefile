
default: compile
	echo "default in Makefile for pwrusb is to compile, but not install"
	echo "...compiling..."
	
Mac.zip:
# can switch back-and-forth to pulling with curl versus the local copy for saving time during development
	curl -O http://www.pwrusb.com/downloads/Mac.zip
# 	cp ../pwrusb/Mac.zip .
	
libpowerusb.dylib:	Mac.zip
	unzip -oq Mac.zip
	mv "`find Mac -name libpowerusb.dylib | head -1`" .

PwrUsbCmd.cpp:	Mac.zip
	unzip -oq Mac.zip
	mv "`find Mac -name PwrUsbCmd.cpp | head -1`" .

PwrUSBImp.h:  Mac.zip
	unzip -oq Mac.zip
	mv "`find Mac -name PwrUSBImp.h | head -1`" .

_PwrUsbCmd.cpp:	 PwrUsbCmd.cpp
	sed -e 's/".\/libpowerusb.dylib"/"libpowerusb.dylib"/g' PwrUsbCmd.cpp | sed -e 's/int main/int OldUnusedMain/g' > _PwrUsbCmd.cpp

pwrusb:   _PwrUsbCmd.cpp  PwrUSBImp.h  libpowerusb.dylib  pwrusb-cmdline.cpp
	g++ -fPIC -g -c pwrusb-cmdline.cpp
	g++ pwrusb-cmdline.o -g -framework IOKit -framework CoreFoundation -o pwrusb

compile:  pwrusb
	echo "compiling pwrusb"

install:  compile
	mv -f pwrusb /usr/local/bin/.
	mv -f libpowerusb.dylib /usr/local/lib/.
	
clean:
	rm -f _PwrUsbCmd.cpp
	rm -f *.o
	rm -f *.so
	rm -rf Mac
	rm -f libpowerusb.dylib
	rm -f PwrUsbCmd.cpp
	rm -f PwrUSBImp.h
	rm -f Mac.zip
