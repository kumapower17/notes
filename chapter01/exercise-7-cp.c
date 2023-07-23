#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

#define errorf(__format, ...) \
    fprintf(stderr, (__format), __VA_ARGS__)

#define alloc(_n) calloc((_n), 1)

enum return_code
{
    FOPEN_ERR = 1,
    WRITE_DEST_ERR = 2,
    READ_SRC_ERR = 3,
    STAT_SRC_ERR = 4,
    CHMOD_DEST_ERR = 5,
};

int cp(const char *src, const char *dest)
{
    enum return_code rc = 0;
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0)
    {
        errorf("open file %s error: %s\n", src, strerror(errno));
        rc = FOPEN_ERR;
        return rc;
    }

    struct stat stats;
    if (fstat(src_fd, &stats) != 0)
    {
        errorf("stat file %s error: %s\n", src, strerror(errno));
        rc = STAT_SRC_ERR;
        return rc;
    }

    int dest_fd = open(dest, O_CREAT | O_TRUNC | O_WRONLY, stats.st_mode);
    if (dest_fd < 0)
    {
        errorf("open file %s, error: %s\n", src, strerror(errno));
        close(src_fd);
        rc = FOPEN_ERR;
        return rc;
    }
    if (fchmod(dest_fd, stats.st_mode) != 0)
    {
        errorf("chmod file %s, error: %s\n", src, strerror(errno));
        close(dest_fd);
        close(src_fd);
        rc = CHMOD_DEST_ERR;
        return rc;
    }

    const blksize_t buf_size = stats.st_blksize;
    void *buf = alloc(buf_size);
    if (buf == NULL)
    {
        goto cleanup;
    }

    for (ssize_t n = 0, written = 0; (n = read(src_fd, buf, buf_size)) > 0;)
    {
        written = write(dest_fd, buf, n);
        if (written != n)
        {
            errorf("write to %s, error: %s\n", dest, strerror(errno));
            break;
        }
    }

cleanup:
    free(buf);
    fsync(dest_fd);
    close(dest_fd);
    close(src_fd);

    return rc;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        errorf("Usage: %s src dest\n", argv[0]);
        exit(1);
    }

    const char *src = argv[1];
    const char *dest = argv[2];
    if (cp(src, dest) != 0)
    {
        return 1;
    }
    return 0;
}
