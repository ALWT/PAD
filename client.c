#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#define BUFF_SIZE 200
int sockfd;

void read_data(int sock,char buf[])
{char c;
int i=0,n;
read(sock,&n,sizeof(int));
while(read(sock,&c,1)>0&&i<BUFF_SIZE&&i<n)
{printf("%c",c);
buf[i++]=c;}
buf[i]='\0';}

void send_data(int sock,char buf[],int len)
{char c;
int i=0,n=len;
write(sock,&n,sizeof(int));
while(write(sock,&buf[i++],1)>0&&i<BUFF_SIZE&&i<n);
buf[i]='\0';}

int setaddr(struct sockaddr_in *addr,char inaddr[],u_int32_t a,short sinport) {
struct hostent *h;
memset((void*)addr,0,sizeof(addr));
if(inaddr!=NULL)
{h = gethostbyname(inaddr);
if(h!=NULL)
{//addr->sin_addr.s_addr=*(u_int32_t*)h->h_addr_list[0];
inet_pton(AF_INET, inaddr, &addr->sin_addr);
printf("Address \n");
}}
else
addr->sin_addr.s_addr=a;
addr->sin_family = AF_INET ;
addr->sin_port = htons(sinport);
return 0 ;
}

void end_proc()
{char aux[]="/exit";
write(sockfd,aux,strlen(aux));
exit(0);
}

void start_chat(char aux[],char exec[])
{char val[100],pip[10];
strcpy(val,"");
strcat(val,exec);
strcat(val,"\ ");
strcat(val,aux);
printf("%s",val);
execlp("gnome-terminal","gnome-terminal","-e",val,NULL);}

int main(int argv,char *args[])
{char text[100];
int j,pid,i;
struct sockaddr_in loc,rem;
if(argv>3&&(pid=fork())==0)
{printf("args2 %s\n",args[2]);
start_chat(args[2],args[3]);
exit(1);
}
int val;
do{fflush(stdin);
fgets(text,100,stdin);
for(i=0;i<20;i++)
{setaddr(&loc,NULL,INADDR_ANY,0);
if ((sockfd=socket(PF_INET,SOCK_STREAM,0)) < 0)
   {printf ("error1 ...\n"); continue;}
printf("1%s\n",strerror(errno));
if ((bind(sockfd,(struct sockaddr *)&loc,sizeof(loc)))<0)
   {printf ("error2 ...\n"); continue;}
setaddr(&rem,args[1],0,atoi(args[2]));
printf("con %d\n",val=connect(sockfd,(struct sockaddr *)&rem,sizeof(rem)));
printf("2%s\n",strerror(errno));
if(val<0)
continue;
send_data(sockfd,text,strlen(text));
printf("sock %d %s",sockfd,text);
//text[strlen(text-1)]='\0';
close(sockfd);
break;
}
}while(strcmp(text,"~exit\n")!=0);
return 0;}
