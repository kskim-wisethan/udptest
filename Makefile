TARGET = udptest
OBJS = main.o linux_udpsender.o linux_udpreceiver.o
CC = -lpthread

$(TARGET): $(OBJS)
	     g++ -o $@ main.o linux_udpsender.o linux_udpreceiver.o $(CC)

main.o: main.cpp
	    g++ -c main.cpp

linux_udpsender.o: linux_udpsender.cpp
	    g++ -c linux_udpsender.cpp

linux_udpreceiver.o: linux_udpreceiver.cpp
	    g++ -c linux_udpreceiver.cpp

clean: 
	    rm -f $(TARGET) $(OBJS)
