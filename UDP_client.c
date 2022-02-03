#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h> // sockets

#define BUFFSIZE 512
#define IP_ADDRESS "127.0.0.1"
#define PORT 3000



typedef struct UDP_Packet {
  char msg[BUFFSIZE];
  int flag;
}UDP_Packet;


int main(int argc, char** argv)
{

int socketFd;
UDP_Packet recvPacket, sendPacket;
int rc, adrlen, lastPacketNr;

struct sockaddr_in clientToSv;

adrlen = sizeof(clientToSv);

bzero(&clientToSv,adrlen);

socketFd = socket(AF_INET,SOCK_DGRAM, 0); //0 sau IPPROTO_UDP la fel
if(socketFd == -1)
{
    printf("Socket %d\n", __LINE__);
    exit(1);
}

clientToSv.sin_family = AF_INET;
clientToSv.sin_port = htons(PORT);
clientToSv.sin_addr.s_addr = inet_addr(IP_ADDRESS);


FILE* f = fopen(argv[1], "r");
char buffer[BUFFSIZE];
  
strcpy(sendPacket.msg, argv[1]);
  
rc = sendto(socketFd, &sendPacket, sizeof(sendPacket), 0,
            (struct sockaddr*)&clientToSv, adrlen);
            
printf("Send file... \n");

if(rc < 0)
{
   printf("Filename");
   exit(1);
}

while(fgets(buffer, BUFFSIZE, f))
{
    SEND_AGAIN:

    strcpy(sendPacket.msg, buffer);

    rc = sendto(socketFd, &sendPacket, sizeof(sendPacket), 0,
            (struct sockaddr*)&clientToSv, adrlen);

    if(rc < 0)
    {
        printf("Sendto %d\n", __LINE__);
        goto SEND_AGAIN;
    }

    usleep(1000);
s
    rc = recvfrom(socketFd, &recvPacket, sizeof(recvPacket), MSG_DONTWAIT,
        (struct sockaddr*)&clientToSv, &adrlen);
        
    if(rc < 0 || recvPacket.flag != 1)
    {
        printf("Recvfrom %d\n", __LINE__);
        goto SEND_AGAIN;
    }

}


sendPacket.flag = 2;

rc = sendto(socketFd, &sendPacket, sizeof(sendPacket), 0,
          (struct sockaddr*)&clientToSv, adrlen);
          
if(rc < 0)
{
    printf("Sendto %d\n", __LINE__);
    exit(1);
}

printf("\n File sent!");

fclose(f);
close(socket);

return 0;

}
