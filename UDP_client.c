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
  int packetNr;

}UDP_Packet;

void Eroare(char *e) { perror(e); exit(1); }

int main(int argc, char** argv)
{
  if(argc!=2)
  {
    printf("Please give an input file!");
    exit(0);
  }


int socketFd;
  UDP_Packet recvPacket, sendPacket;
  int rc, adrlen, lastPacketNr;

struct sockaddr_in clientToSv;

  adrlen = sizeof(clientToSv);
  lastPacketNr=0;
  bzero(&clientToSv,adrlen);

  socketFd = socket(AF_INET,SOCK_DGRAM, 0); //0 sau IPPROTO_UDP la fel
  if(socketFd == -1)
  {
    Eroare("Socket fd");
  }

  clientToSv.sin_family = AF_INET;
  clientToSv.sin_port = htons(PORT);
  clientToSv.sin_addr.s_addr = inet_addr(IP_ADDRESS);


  FILE* f = fopen(argv[1], "r");
  char buffer[BUFFSIZE];

  while(fgets(buffer, BUFFSIZE, f))
  {
    SEND_AGAIN:

    sendPacket.packetNr = lastPacketNr;
    strcpy(sendPacket.msg, buffer);

    rc = sendto(socketFd, &sendPacket, sizeof(sendPacket), 0,
            (struct sockaddr*)&clientToSv, adrlen);

            if(rc < 0)
            {
              printf("Send probl");
              exit(1);
            }

    usleep(10000);

    rc = recvfrom(socketFd, &recvPacket, sizeof(recvPacket), MSG_DONTWAIT,
        (struct sockaddr*)&clientToSv, &adrlen);
        if(rc < 0)
        {
          printf("Rcv probl");
          goto SEND_AGAIN;
        }
    if(recvPacket.packetNr != lastPacketNr || recvPacket.flag != 1)
    {
      goto SEND_AGAIN;
    }
    lastPacketNr++;
  }

  sendPacket.packetNr = lastPacketNr;
  sendPacket.flag = 2;

  rc = sendto(socketFd, &sendPacket, sizeof(sendPacket), 0,
          (struct sockaddr*)&clientToSv, adrlen);
if(rc < 0)
{
    printf("Send probl final packet");
    exit(1);
}

printf("File sent!");
fclose(f);
close(socket);

return 0;

}
