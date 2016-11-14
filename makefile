CC=gcc
CFLAGS=-g -Wall -O2
LDFLAGS=-L/usr/lib/
LIBS=-L/usr/lib/ -lmysqlclient #-lpthread
INCLUDES= -I./mysql-connector-c-6.0.2-linux-rhel5
TARGET_CLIENT=sendMsg
all:$(TARGET_CLIENT)

$(TARGET_CLIENT): utils.o
	$(CC) -o $@ $(INCLUDES) $(CFLAGS) utils.o sendMsg.c $(LIBS)

utils.o: utils.c
	$(CC) $(CFLAGS) $(INCLUDES) -c utils.c

clean:
	rm  -f *.o $(TARGET_CLIENT)
