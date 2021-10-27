=============
DIP Distribution ${version}
=============

IMPORTANT NOTE:
---------------

DIP 5.4.1 API requires recompilation of users' applications. The DipInt type changed from "long" to "int" for portability
on Linux 64bits platforms. (No truncation impact on 32bits platforms.)

DIP 5.4.0 API and later allows to publish DIP Publications without restrictions on fields numbers (previously 
limited to 32 fields per DIP Publications).
Hence the usage of more than 32 fields is still not recommended for backward compatibility reasons.


--------
Content:
--------

The DIP delivery contains 4 directories:

/dip-${version}		DIP base location.
	/doc		DIP API Generated Javadoc
	/include	C++ API include files.
	/lib		Java and C++ libraries.
	/lib64		Native libraries for AMD64 platforms
	/tools		DIP browser (executable jar file).

-----------------
More information:
-----------------

DIP Browser:
------------
- On Windows, ensure that the PATH environment variable includes "<DIP Installation Directory>\lib"
- On Linux, ensure to source the environment, by calling "source setenv.sh" or  "source setenv64.sh" from the <DIP Installation Directory> (or "source setenv64.sh" for AMD-64 platforms).
- To run the DIP Browser, you can use the provided shell script (browser.sh or browser.bat)


All:
----
- The Java software provided here was compiled with SUN J2SE 1.7 rev 6 
for Java 1.6 (compiler target) 
- This Java runtime version 1.6 or later is therefore required. Check your Java 
runtime version using the command: java -version


Windows:
--------
- The DIP libraries and samples are generated with MS Visual Studio .Net 2010.

Linux:
------ 
- The DIP libraries and samples are generated with 
   - GCC 4.4.7 on SLC6 for 32 bit platforms
   - GCC 4.4.7 on SLC6 for AMD 64 bit platforms
- In case you recompile the C++ examples on SLC6. The compilation warning about 
the standard libraries are not affecting their behavior.
- To run the setenv.sh script, open a bash shell and type "source ./setenv.sh"
- For AMD 64-bit, run the setenv64.sh script, open a bash shell and type "source ./setenv64.sh"
- If you want to use the new DIP library with existing C++ programs, you need to
rename the new DIP library or recompile your program.
- The tar file provided cuts away the access rights (Access Control List) for 
the files it contains. You will need to set them back properly for the 
executables and the libraries.
- You may need to open TCP ports 5100 to 6000 in the Linux machine running a DIP
server to make the latter visible from DIP Clients.

-----------------
Troubleshooting:
-----------------
For any troubles, please report to icecontrols.support@cern.ch

