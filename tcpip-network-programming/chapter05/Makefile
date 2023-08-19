all: opserver opclient
CC ?= gcc

opserver: op_server.c
	$(CC) $^ -o $@
opclient: op_client.c
	$(CC) $^ -o $@

.PHONY: all
