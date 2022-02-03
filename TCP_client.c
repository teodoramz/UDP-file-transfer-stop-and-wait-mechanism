#include <sys/socket.h>

#include <netinet/in.h>

#include <sys/types.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>



void Eroare(char *e) { perror(e); exit(1); }



int main() {

int sock;

struct sockaddr_in server;

sock=socket(AF_INET,SOCK_STREAM,0);

bzero(&server,sizeof(server));

server.sin_family=AF_INET;

inet_pton(AF_INET,"127.0.0.1", &server.sin_addr);

server.sin_port=htons(2500);

int status=connect(sock,(struct sockaddr*) &server, sizeof(server));

if (status==0)

printf("Sunteti conectat! \n");

else 	Eroare("Eroare de conexiune! Conexiunea nu s-a putut realiza.");

char sendBuff[32];

char receiveBuff[32];

do {

printf("Comanda ");

scanf("%s",sendBuff);

send(sock, &sendBuff,32,0);

recv(sock,&receiveBuff,32,0);

printf("%s\n",receiveBuff);

} while(strcmp(sendBuff,"exit")!=0);



shutdown(sock,SHUT_RDWR);

close(sock);

}
