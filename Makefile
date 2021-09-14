#Ansi flag?
CC = g++
CFLAGS = -Wall -ansi -I ../dip-5.7.0/include -D_GLIBCXX_USE_CXX11_ABI=0 

DLIB = -l dip -l log4cplus -lpthread -ldl

all: Client

Client: client.cpp ../dip-5.7.0/lib64/libdip.so
	$(CC) $(CFLAGS) client.cpp -L../dip-5.7.0/lib64 $(DLIB) -o client
