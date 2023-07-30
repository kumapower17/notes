#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in addr1;
    unsigned long conv_addr = inet_addr("127.1");
    if (conv_addr == INADDR_NONE)
    {
        printf("Error occured! \n");
    }
    else
    {
        addr1.sin_addr.s_addr = conv_addr;
        char *s = inet_ntoa(addr1.sin_addr);
        printf("Network ordered integer addr: %s \n", s);
    }
    return 0;
}
