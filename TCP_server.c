#include <sys/socket.h>

#include <sys/types.h>

#include <netinet/in.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <time.h>

#include <sys/utsname.h>





void Eroare(char *e) { perror(e); exit(1); }

void getTime(char*b)

{

time_t timp=time(NULL);

struct tm *tinfo;

tinfo=localtime(&timp);

sprintf(b,"%d:%d:%d",tinfo->tm_hour, tinfo->tm_min, tinfo->tm_sec);

}

void getDate(char* b)

{

time_t timp=time(NULL);

struct tm *tinfo;

tinfo=localtime(&timp);

sprintf(b,"%d.%d.%d",tinfo->tm_mday,tinfo->tm_mon, tinfo->tm_year+1900);

}

void getOs(char *b)

{

struct utsname os;

uname(&os);

char* s= (char*)malloc(strlen(os.sysname)+strlen(os.release)+2);

strcpy(s,os.sysname);

strcat(s," ");

strcat(s,os.release);

strcpy(b,s);

}



void main()

{

struct sockaddr_in server;

struct sockaddr_in client;

int readSock, connSock;

readSock=socket(AF_INET, SOCK_STREAM,0);

bzero(&server, sizeof(server));

server.sin_family=AF_INET;

server.sin_addr.s_addr=htonl(INADDR_ANY);

server.sin_port=htons(2500);



bind(readSock,(struct sockaddr*) &server, sizeof(server));

listen(readSock,10);

while(1) {

int length=sizeof(client);

connSock=accept(readSock, (struct sockaddr*) &client,&length);

if(!fork()) {

close(readSock);

char receiveBuff[32],sendBuff[32];

int status;

do {

status=recv(connSock, &receiveBuff, 32,0);

printf("%d",status);

printf("%s\n", receiveBuff);

if (strcmp(receiveBuff,"timp")==0) {getTime(sendBuff);}

else if(strcmp(receiveBuff, "data")==0) { getDate(sendBuff);}

else if(strcmp(receiveBuff,"os")==0) { getOs(sendBuff);}

else strcpy(sendBuff,"Necunoscut");

send(connSock,&sendBuff,32,0);

} while(status!=0);

close (connSock);

exit(0);

}

close(connSock);

}

close(readSock);

}
