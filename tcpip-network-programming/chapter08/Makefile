all: hostname hostaddr
CC ?= gcc

hostname: gethostbyname.c
	$(CC) $^ -o $@
hostaddr: gethostbyaddr.c
	$(CC) $^ -o $@
.PHONY: all
