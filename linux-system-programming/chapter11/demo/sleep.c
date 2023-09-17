#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void ignore(int signum) {}

int main()
{
    // for demo only
    signal(SIGUSR1, ignore);
    unsigned n = sleep(10);
    printf("remaining: %u secs\n", n);
}
