all: uecho_server uecho_client host1 host2
CC ?= gcc

uecho_server: uecho_server.c
	$(CC) $^ -o $@
uecho_client: uecho_client.c
	$(CC) $^ -o $@
host1: bound_host1.c
	$(CC) $^ -o $@
host2: bound_host2.c
	$(CC) $^ -o $@
.PHONY: all
