
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
    
This will install the executable (`pwrusb`) in `/usr/local/bin/` and the necessary library (`libpowerusb.dylib`) in `/usr/local/lib/'.

## Usage

Example usage:

*Get status:*

    pwrusb status

returns, e.g. (where 0 means Off & 1 means On for outlets 1 2 3):

    Outlet states on bank 0:
        0  0  0

*Set all 3 outlets individually:*

    pwrusb set 0 1 0

where 0 is Off & 1 (or anything other than 0) is On for outlets 1 2 3

*Set all outlets to on/off together:*

    pwrusb setall 0

where 0 is Off & 1 (or anything other than 0) is On

*Set one outlet on/off:*

    pwrusb setone 3 0
    
where first number is outlet number (1 2 or 3) and second is 0 for Off or 1 for On (or anything other than 0)

*Read the total current draw in milliamp:*

    pwrusb current
    
Output looks like:

    Total current (milliamp) on bank 0: 40

Note that will almost never be 0, even if outlets are off.

Note that outlets respond & are controllable even if overall powerstrip red power switch is off.




        
## Additional Usage Notes

Lighted power switch on pwrusb.com strips controls power to outlets, but does not power on/off the USB controller within the strip.  When the physical switch is off, strip should report a low (but not zero) current draw and will report what state the outlets will be when the physical switch is flipped back on, i.e. can report True even when no power is coming out of an outlet.

# History

This package was assembled in mid-2016 from earlier more complicated versions by hroe.  See `CHANGES.txt` for change log.