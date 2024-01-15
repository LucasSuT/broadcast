#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#define BROADCAST_IP "255.255.255.255"

int broadcast_send()
{
    int fd,ret,on;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    on = 1;
    ret = setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &on,sizeof(on));
    if(ret < 0)
    {
        printf("setsockopt failed !\n");
        return 1;
    }

    memset(&addr, '0', sizeof(addr)); 

    addr.sin_family = AF_INET;
    addr.sin_port = htons(7000); 
    addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    ret = sendto(fd,"test sendto",strlen("test sendto"),0,(struct sockaddr *)&addr,sizeof(addr));
    if(ret < 0)
    {
        printf("boardcast failed !\n");
        return 1;
    }
    printf("send %d\n",ret);

    printf("send OK\n");
    close(fd);
    return 0;
}

int broadcast_recv()
{
    int fd,ret,on;
    struct sockaddr_in addr, addr_from;
    char message[512];
    socklen_t len;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    on = 1;
    ret = setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &on,sizeof(on));
    if(ret < 0)
    {
        printf("setsockopt failed !\n");
        return 1;
    }

    memset(&addr, 0, sizeof(addr)); 

    addr.sin_family = AF_INET;
    addr.sin_port = htons(7000); 
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        printf("bind failed !\n");
        return 1;
    }

    memset(message, 0x0,sizeof(message));

    len = sizeof(addr_from);
    ret = recvfrom(fd, message, sizeof(message), 0, (struct sockaddr *)&addr, &len);
    if(ret < 0)
    {
        printf("recv failed !\n");
        return 1;
    }

    printf("recv OK\n");
    close(fd);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        if(strcmp(argv[1],"recv") == 0)
            broadcast_recv();
        else if(strcmp(argv[1],"send") == 0)
            broadcast_send();
    }
    else printf("argc error!\n");
    return 0;
}