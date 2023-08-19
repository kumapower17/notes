#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define errorf(__format, ...) \
    fprintf(stderr, (__format), __VA_ARGS__)

#define alloc(_n) calloc((_n), 1)

enum return_code
{
    FOPEN_ERR = 1,
    WRITE_DEST_ERR = 2,
    READ_SRC_ERR = 3,
};

int cp(const char *src, const char *dest)
{
    enum return_code rc = 0;
    FILE *src_file = fopen(src, "r");
    if (src_file == NULL)
    {
        errorf("open file %s error: %s\n", src, strerror(errno));
        rc = FOPEN_ERR;
        return rc;
    }

    FILE *dest_file = fopen(dest, "wb");
    if (dest_file == NULL)
    {
        errorf("open file %s, error: %s\n", src, strerror(errno));
        fclose(src_file);
        rc = FOPEN_ERR;
        return rc;
    }

    const int buf_size = 1 << 15;
    void *buf = alloc(buf_size << 1);
    if (buf == NULL)
    {
        goto cleanup;
    }
    setvbuf(src_file, buf, _IOFBF, buf_size);
    setvbuf(dest_file, &buf[buf_size], _IOFBF, buf_size);

    for (int c; (c = getc(src_file)) != EOF;)
    {
        if (putc(c, dest_file) == EOF)
        {
            errorf("write to dest, %s error: %s\n", dest, strerror(errno));
            rc = WRITE_DEST_ERR;
            goto cleanup;
        }
    }
    if (ferror(src_file))
    {
        errorf("read src %s, error: %s\n", dest, strerror(errno));
        rc = READ_SRC_ERR;
        goto cleanup;
    }

cleanup:
    free(buf);
    if (dest_file != NULL)
    {
        fflush(dest_file);
    }
    fclose(dest_file);
    fclose(src_file);

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
