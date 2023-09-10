#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

static void display_mallocinfo2(void)
{
    struct mallinfo2 mi;
    mi = mallinfo2();

    printf("\nTotal non-mmapped bytes (arena):       %zu\n", mi.arena);
    printf("# of free chunks (ordblks):            %zu\n", mi.ordblks);
    printf("# of free fastbin blocks (smblks):     %zu\n", mi.smblks);
    printf("# of mapped regions (hblks):           %zu\n", mi.hblks);
    printf("Bytes in mapped regions (hblkhd):      %zu\n", mi.hblkhd);
    printf("Max. total allocated space (usmblks):  %zu\n", mi.usmblks);
    printf("Free bytes held in fastbins (fsmblks): %zu\n", mi.fsmblks);
    printf("Total allocated bytes (uordblks):      %zu\n", mi.uordblks);
    printf("Total free bytes (fordblks):           %zu\n", mi.fordblks);
    printf("Topmost releasable bytes (keepcost):   %zu\n", mi.keepcost);
}

static void display_brk(void)
{
    void *p = sbrk(0);
    printf("brk %p\n", p);
}

int main()
{
    display_brk();
    display_mallocinfo2();

    (void)getchar();
    char *mems[1024];
    for (int i = 0; i < sizeof(mems) / sizeof(mems[0]); i++)
    {
        const size_t alloc_size = 9 * 1024;
        char *p = malloc(alloc_size);
        memset(p, 1, alloc_size);
        mems[i] = p;
    }

    display_brk();
    display_mallocinfo2();

    (void)getchar();
    for (int i = 0; i < sizeof(mems) / sizeof(mems[0]); i++)
    {
        if ((i % 5) != 0)
        {
            free(mems[i]);
            mems[i] = NULL;
        }
    }

    display_brk();
    display_mallocinfo2();

    (void)getchar();
    for (int i = 0; i < sizeof(mems) / sizeof(mems[0]); i++)
    {
        free(mems[i]);
    }

    display_brk();
    display_mallocinfo2();

    (void)getchar();
    return 0;
}
