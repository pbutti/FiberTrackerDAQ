CC = g++
#No ANSI flag, as we use various c++11 stuff. Hope DIP is okay with that. 
CFLAGS = -Wall -I ../dip-5.7.0/include -D_GLIBCXX_USE_CXX11_ABI=0 

DLIB = -l dip -l log4cplus -lpthread -ldl

all: Client

Client: client.cpp ../dip-5.7.0/lib64/libdip.so
	$(CC) $(CFLAGS) client.cpp trackerEvent.cpp -L../dip-5.7.0/lib64 $(DLIB) -o client
