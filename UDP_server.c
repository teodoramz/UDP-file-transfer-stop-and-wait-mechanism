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
  int packetNr;

}UDP_Packet;

void Eroare(char *e) { perror(e); exit(1); }

int main(int argc, char** argv)
{
  if(argc!=2)
  {
    printf("Please give an output file!");
    exit(0);
  }

int socketFd;
UDP_Packet recvPacket, sendPacket;
int rc, adrlen, lastPacketNr;

struct sockaddr_in serverToClient;
struct sockaddr_in clientToSv;

adrlen = sizeof(clientToSv);
lastPacketNr=0;
bzero(&serverToClient,sizeof(serverToClient));

socketFd = socket(AF_INET,SOCK_DGRAM, 0); //0 sau IPPROTO_UDP la fel
if(socketFd == -1)
{
  Eroare("Socket fd");
}

serverToClient.sin_family = AF_INET;
serverToClient.sin_port = htons(PORT);
serverToClient.sin_addr.s_addr =  htonl(INADDR_ANY);

rc = bind(socketFd,
   (struct sockaddr*)&serverToClient,
    sizeof(serverToClient));

if(rc == -1)
{
  Eroare(" binding");
}

FILE* f = fopen(argv[1], "w");

while(1)
{
  rc = recvfrom(socketFd, &recvPacket, sizeof(recvPacket), 0,
  (struct sockaddr *)&clientToSv, &adrlen);
  if(rc == -1)
  {
    Eroare("receive");
  }

  if(recvPacket.flag == 2)
  {
    break;
  }

  sendPacket.packetNr = lastPacketNr;

  if(lastPacketNr == recvPacket.packetNr)
  {
    fputs(recvPacket.msg, f);
    memset(&(recvPacket.msg[0]),0, sizeof(recvPacket.msg));
    lastPacketNr++;

    sendPacket.flag = 1;
  }

  rc = sendto(socketFd, &sendPacket, sizeof(sendPacket),
  0,(struct sockaddr*)&clientToSv, adrlen);
  if(rc == -1)
  {
    Eroare("send to");
  }
}

printf("Done!\n");

fclose(f);
close(socketFd);

return 0;

}
