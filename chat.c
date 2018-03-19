#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
int sockfd, newsockfd;
int total=0, no_log=0;

struct client
{
	struct client *urm;
	int s;
	pthread_t t;
	char user[100]; 
}*rad;

struct data
{
	char user[100], pass[100];
}c[100];

struct client *find_user(char *user)
{
	struct client *i;
	for(i=rad; i!=NULL; i=i->urm)
		if(strcmp(i->user, user)==0)
			break;
	return i;
}

int check_user(char *user, char *pass)
{
	int i;
	for(i=0; i<no_log; i++)
      		if(strcmp(c[i].user, user)==0 && strcmp(c[i].pass, pass)==0)
          		return 1;
  	return 0;
}

int remove_client(int s)
{
	struct client *i,*q;
	if(rad==NULL)
		return -1;
  
	for(i=rad, q=NULL; i!=NULL && i->s!=s; q=i, i=i->urm);

  	if(i!=NULL)
  	{
	    total--;
	    if(i==rad)
	     	rad=rad->urm;
	    else 
		q->urm=i->urm;

	    close(i->s);
	    free(i);
	    return 0;
	}
  	
	return -1;
}

int setaddr(struct sockaddr_in *addr, char inaddr[], u_int32_t a, short sinport) 
{
	struct hostent *h;
	memset((void*)addr,0,sizeof(addr));

  	if(inaddr!=NULL)
  	{
    		h = gethostbyname(inaddr);
    		if(h!=NULL)
      			addr->sin_addr.s_addr=*(u_int32_t*)h->h_addr_list[0];
  	}
    	else
      		addr->sin_addr.s_addr=a;

    	addr->sin_family = AF_INET ;
    	addr->sin_port = htons(sinport);
    	return 0 ;
}

int read_data(int sock,char buf[])
{
	int n, y, z;
	y=read(sock, &n, sizeof(int));
	z=read(sock, buf, n);
	buf[n]='\0';
	return y>0 && z>0;
}

void send_data(int sock,char buf[])
{
	int n=strlen(buf);
	write(sock, &n, sizeof(int));
	write(sock, buf, n);
}

void read_file(char file_name[])
{
	FILE *f=fopen(file_name,"rt");
	printf("Clients:\n");
  	while(!feof(f))
	{
	    fscanf(f, "%s %s\n", c[no_log].user, c[no_log].pass);
	    printf("%s %s\n", c[no_log].user, c[no_log].pass);
	    no_log++;
	}
  	fclose(f);
}

void *thread_start(void *arg)
{ 
	char line[100], mess[150],client[100];
  	struct client *i, *s=(struct client *)arg;
	
	while(3)
	{
		
		strcpy(mess, s->user);
	     	strcat(mess, ": ");
	      	read_data(s->s, line);
	      	strcat(mess, line);
		
		for(i=rad; i!=NULL; i=i->urm)
	      	{
			send_data(i->s, mess);
			printf("line: %s \n", mess);
	      	}
		if(total==1)
				send_data(s->s,"You are alone.");
	      	if(strcmp(line, "~exit") == 0)
	      	{
		  	printf("Client log out: %s\n", s->user);
			remove_client(s->s);
			break;
	      	}
		

		
  	}
  	
	if(rad==NULL)
  	{
    		close(sockfd);
		exit(0);
  	}

   	return NULL; 
}

int main(int argv, char *args[])
{
	char name[30], line[200], user[100], pass[100];
  	int j;
  	struct client *p;
	struct sockaddr_in loc, rem;
	
	if(argv<3)
	{
	      printf("Format %s ip_addr port_no user_file\n", args[0]);
	      exit(0);
	}

	read_file(args[2]);
  	rad=NULL;
	printf("%s\n", args[1]);
  
  	socklen_t rlen=sizeof(rem);
  	setaddr(&loc, NULL, INADDR_ANY, atoi(args[1]));

  	//creare server
  	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  	{
    		printf ("eroare la creare server\n"); 
		exit(1);
  	}

  	if ((bind(sockfd, (struct sockaddr *)&loc, sizeof(loc)))<0)
  	{
    		printf ("eroare bind\n"); 
    		exit(1);
  	}
 
  	if (listen(sockfd,10) < 0)
  	{
    		printf ("eroare listen\n"); 
   	 	exit(1);
 	}

  	//server primire cereri
  	for (;;)
  	{
  		newsockfd=accept(sockfd, (struct sockaddr *)&rem, &rlen);/* blocare pentru asteptare cereri*/
   
    		if (newsockfd < 0)
      			printf ("eroare accept\n");
    		else
    		{
			read_data(newsockfd, user);
        		read_data(newsockfd, pass);
        		
			//verif user
        		if(check_user(user, pass) && find_user(user)==NULL)
        		{
				total++;
			   	p=malloc(sizeof(struct client));
			    	p->s=newsockfd;
			    	p->urm=NULL;
			    	strcpy(p->user, user);

            			if(rad==NULL)
                			rad=p;
           			else 
           			{
                			p->urm=rad;
                			rad=p;
            			}

            			send_data(newsockfd, "okay");
            			printf("Client login : %s,%d\n", p->user,p->s);
				
            			pthread_create(&rad->t, NULL, &thread_start, (void *)p);
        		}
       
			//user nu exista
        		else 
        		{
            			
            			close(newsockfd);
        		}
    		}
  	}
  	return 0;
}
