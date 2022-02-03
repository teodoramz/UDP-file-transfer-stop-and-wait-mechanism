#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

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

struct sockaddr_in serverToClient;
struct sockaddr_in clientToSv;

adrlen = sizeof(clientToSv);

bzero(&serverToClient,sizeof(serverToClient));

socketFd = socket(AF_INET,SOCK_DGRAM, 0); //0 sau IPPROTO_UDP la fel

if(socketFd == -1)
{
  printf("Socket %d\n", __LINE__);
  exit(1);
}

serverToClient.sin_family = AF_INET;
serverToClient.sin_port = htons(PORT);
serverToClient.sin_addr.s_addr =  htonl(INADDR_ANY);

rc = bind(socketFd, (struct sockaddr*)&serverToClient, sizeof(serverToClient));

if(rc == -1)
{
  printf("Bind %d\n", __LINE__);
  exit(1);
}


rc = recvfrom(socketFd, &recvPacket, sizeof(recvPacket), 0,
  (struct sockaddr *)&clientToSv, &adrlen);
  if(rc == -1)
  {
    printf("Recvfrom %d\n", __LINE__);
    exit(1);
  }
  printf("%s", recvPacket.msg);

FILE* f = fopen(recvPacket.msg , "w");

while(1)
{
  sendPacket.flag = 1;
  
  rc = recvfrom(socketFd, &recvPacket, sizeof(recvPacket), 0,
  		(struct sockaddr *)&clientToSv, &adrlen);
if(rc == -1)
{
    printf("Recvfrom %d\n", __LINE__);
    sendPacket.flag = 3;
    
}

if(recvPacket.flag == 2)
{
    break;
}


if(sendPacket.flag == 1){
   fputs(recvPacket.msg, f);

   memset(&(recvPacket.msg[0]),0, sizeof(recvPacket.msg));
}




  rc = sendto(socketFd, &sendPacket, sizeof(sendPacket),
  	0,(struct sockaddr*)&clientToSv, adrlen);
  if(rc == -1)
  {
    printf("SendTo %d\n", __LINE__);
    exit(1);
  }
  
}

printf("Done!\n");

fclose(f);
close(socketFd);

return 0;

}
