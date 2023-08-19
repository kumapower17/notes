all: hserver hclient lopen lread fds
CC ?= gcc
hserver: hello_server.c
	$(CC) $^ -o $@

hclient: hello_client.c
	$(CC) $^ -o $@
lopen: low_open.c
	$(CC) $^ -o $@

lread: low_read.c
	$(CC) $^ -o $@

fds: fd_seri.c
	$(CC) $^ -o $@

.PHONY: all
