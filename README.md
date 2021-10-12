# FiberTrackerDAQ
DIP Client for the XBPF fiber trackers

## Build instructions
Clone this repo:
    git clone https://github.com/Skelpdar/FiberTrackerDAQ.git

Download DIP 5.7.0 from https://readthedocs.web.cern.ch/display/ICKB/DIP+API and extract it into the same directory as the repo.

Add the path to the DIP libraries:
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/dip-5.7.0/lib64

Compile it:
    cd FiberTrackerDAQ
    make

Then it should run:
    ./client
