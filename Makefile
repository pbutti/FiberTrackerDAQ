CC = g++
#No ANSI flag, as we use various c++11 stuff. Hope DIP is okay with that. 
CFLAGS = -Wall -I ../dip-5.7.0/include -D_GLIBCXX_USE_CXX11_ABI=0 -I include/

DLIB = -l dip -l log4cplus -lpthread -ldl

all: Client ClientBinary ReadBinary

Client: src/client.cpp ../dip-5.7.0/lib64/libdip.so
	$(CC) $(CFLAGS) src/client.cpp -L../dip-5.7.0/lib64 -L/usr/lib64/root $(DLIB) -I/usr/include/root/ -o client 

ClientBinary: src/clientBinary.cpp ../dip-5.7.0/lib64/libdip.so
	$(CC) $(CFLAGS) src/clientBinary.cpp -L../dip-5.7.0/lib64 -L/usr/lib64/root $(DLIB) -I/usr/include/root/ -o clientBinary 

ReadBinary: src/readBinary.cpp
	$(CC) src/readBinary.cpp -o readBinary 
