CC = g++


#CFLAGS = -c -Wall -DDEBUG -g 
CFLAGS=-O3 -std=c++0x -pg -D_DEBUG -g -c -Wall

LDFLAGS =

FUNCTIONS = project.cpp   #3variables
SERVER = node.cpp
CLIENT = peer.cpp

FUNCTIONS_OBJECTS = $(FUNCTIONS:.cpp=.o)
SERVER_OBJECTS = $(SERVER:.cpp=.o)
CLIENT_OBJECTS = $(CLIENT:.cpp=.o)

SERVER_EXE = NODE	#exe files
CLIENT_EXE = PEER

.PHONY: all Servers Clients

all: Servers Clients

Servers: $(SERVER_EXE)

Clients: $(CLIENT_EXE)

$(SERVER_EXE): $(FUNCTIONS_OBJECTS) $(SERVER_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(CLIENT_EXE): $(FUNCTIONS_OBJECTS) $(CLIENT_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
