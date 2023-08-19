all: eserver eclient
CC ?= gcc

eserver: echo_server.c
	$(CC) $^ -o $@
eclient: echo_client.c
	$(CC) $^ -o $@

.PHONY: all
