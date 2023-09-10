#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>

void handler(int signum)
{
}

int main()
{
    struct sigaction sa = {};
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    (void)sigaction(SIGUSR1, &sa, NULL);

    // 64m
    size_t alloc_size = 64ul * 1024 * 1024;
    unsigned char *p = malloc(alloc_size);
    pause();

    memset(p, 0x1, alloc_size);
    pause();
    return 0;
}
