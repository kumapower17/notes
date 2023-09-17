#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

static void print_time(const char *field, time_t t)
{
    // The buffer must be at least 26 characters in length.
    char buf[32] = {0};
    struct tm tm = {0};
    ctime_r(&t, buf);
    printf("%s: %s", field, buf);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        eprintf("usage: xstat <file>\n");
        exit(1);
    }

    const char *filename = argv[1];
    struct statx stats = {0};
    if (statx(AT_FDCWD, filename, AT_STATX_SYNC_AS_STAT | AT_SYMLINK_NOFOLLOW, STATX_BTIME, &stats))
    {
        eprintf("xstat %s, %s\n", filename, strerror(errno));
        exit(1);
    }
    print_time("birth_time", stats.stx_btime.tv_sec);
}
