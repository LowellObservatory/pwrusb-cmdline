
# pwrusb

A simple command-line interface for controlling pwrusb.com power strip outlets.

After multiple attempts at a more flexible (but more complicated) interface, decided that it was better to back out to the simplest command-line interface that re-establishes USB connectivity with each call, versus trying to maintain USB connectivity and fall prey to occasional USB dropouts from the pwrusb strip.

## Installation notes

To avoid copyright issues, several files are not included in this distribution.  Makefile will attempt to download and install these files automatically.  Or, they can be downloaded from the Mac software distribution from the [pwrusb website](http://www.pwrusb.com/downloads.html).  These files are:
    
    PwrUSBImp.h
    PwrUsbCmd.cpp
    libpowerusb.dylib

A version of libusb-1.0 will need to be installed.  One straightforward way to get this is via [homebrew](http://brew.sh/) with:

    brew install libusb
            
## Typical Installation

From command line:
    
    make
    make install

## Usage

Example usage:

    TK
        
## Additional Usage Notes

Lighted power switch on pwrusb.com strips controls power to outlets, but does not power on/off the USB controller within the strip.  When the physical switch is off, strip should report a low (but not zero) current draw and will report what state the outlets will be when the physical switch is flipped back on, i.e. can report True even when no power is coming out of an outlet.

# History

This package was assembled in mid-2016 from earlier more complicated versions by hroe.  See `CHANGES.txt` for change log.