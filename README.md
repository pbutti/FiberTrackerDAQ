# FiberTrackerDAQ
DIP Client for the XBPF fiber trackers

## Build instructions
Clone this repo:

    git clone https://github.com/Skelpdar/FiberTrackerDAQ.git

Download DIP 5.7.0 from https://readthedocs.web.cern.ch/display/ICKB/DIP+API and extract it into the same directory as the repo.

Add the path to the DIP libraries:

    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/dip-5.7.0/lib64

#Compilation

    mkdir build; cd build
    cmake -DDIP_Dir=/full/path/to/dip-5.7.0/ -DCMAKE_INSTALL_PREFIX=../install/ ..
    make install

   
## Error and Warning Logging
Configuration for the logging, both to the console and to disk, is done in the log4cplus.properties file. Without this, you won't see any errors or warnings from DIP. More instructions are found at https://readthedocs.web.cern.ch/display/ICKB/DIP+Logging+with+LOG4CPLUS 
