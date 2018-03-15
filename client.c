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

int read_data(int sock,char buf[])
{
	char c;
	int i=0, n, y, z;
	y=read(sock, &n, sizeof(int));
	//printf("n=%d\n",n);
	z=read(sock, buf, n);
	buf[n]='\0';
	//printf("%s\n",buf);
	return y>0 && z>0;
}

void send_data(int sock,char buf[])
{
	char c;
	int i=0, n=strlen(buf);
	//printf("%s %d\n",buf,n);
	write(sock, &n, sizeof(int));
	write(sock, buf, n);
}

int setaddr(struct sockaddr_in *addr,char inaddr[],u_int32_t a,short sinport) 
{
	struct hostent *h;
	memset((void*)addr,0,sizeof(addr));
	if(inaddr!=NULL)
	{
		h = gethostbyname(inaddr);
		if(h!=NULL)
		{
			//addr->sin_addr.s_addr=*(u_int32_t*)h->h_addr_list[0];
			inet_pton(AF_INET, inaddr, &addr->sin_addr);
			//printf("Address \n");
		}
	}
	else
		addr->sin_addr.s_addr=a;
	
	addr->sin_family = AF_INET ;
	addr->sin_port = htons(sinport);
	return 0 ;
}

int main(int argv,char *args[])
{
	char text[100];
	int j, val, pid, i, init=1;
	char user[100], pass[100];
	struct sockaddr_in loc, rem;
	char mess[100];
	int ok=0;
	
	do
	{
		if(init==1)//conectare socket
		{
			if ((sockfd=socket(PF_INET, SOCK_STREAM, 0)) < 0)
		   	{
				printf ("error1 ...\n"); 
				continue;
			}
			//printf("1%s\n", strerror(errno));
			setaddr(&rem, args[1], 0, atoi(args[2]));
			val=connect(sockfd, (struct sockaddr *)&rem, sizeof(rem));
			//printf("con %d\n",val );
			//printf("2%s\n", strerror(errno));
			
			if(val<0)
				break;
	
			else 
			{
				init=0;//logare
				printf("User: ");
				scanf("%s", user);
				printf("Pass: ");
				scanf("%s", pass);

				send_data(sockfd, user);
				send_data(sockfd, pass);
				read_data(sockfd, mess);
				printf("Mess %s|\n", mess);

				if(strcmp(mess,"okay")!=0)
				{
					close(sockfd);
				    	exit(0);
				}
				else ok=1;

	   			//creare proces copil=> primire mesaje
	    		if(fork()==0)
	  				while(3)
					{
						if(ok==1)
						{
							int y=read_data(sockfd, mess);

					   		printf("%s\n", mess);
						   	if(strcmp(mess,"~exit")==0 || y<=0)
						   	{
								close(sockfd);
								exit(0);
							}
						}
					}
			} 
		}
		//trimiteremesaj
		fflush(stdin);
		fgets(text, 100, stdin);
		text[strlen(text)-1]='\0';

		
		send_data(sockfd, text);
		

		//printf("sock %d %s",sockfd,text);

		//iesire client
		if(strcmp(text,"~exit")==0)
		{
			close(sockfd);
			ok=0;
			break;
		}
	}while(3);
	return 0;
}
