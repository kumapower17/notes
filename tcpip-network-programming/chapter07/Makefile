all: fserver fclient
CC ?= gcc

fserver: file_server.c
	$(CC) $^ -o $@
fclient: file_client.c
	$(CC) $^ -o $@
.PHONY: all
