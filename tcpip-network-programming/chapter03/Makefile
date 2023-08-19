all: conv addr aton ntoa
CC ?= gcc

conv: endian_conv.c
	$(CC) $^ -o $@
addr: inet_addr.c
	$(CC) $^ -o $@
aton: inet_aton.c
	$(CC) $^ -o $@
ntoa: inet_ntoa.c
	$(CC) $^ -o $@
.PHONY: all
