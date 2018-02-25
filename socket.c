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
#include <time.h>
#define PORT 20232
#define DEL 2000 
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
 
    // Stroing start time
    clock_t start_time = clock();
 
    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds)
        ;
}
  
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char hello[100];
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    printf("%s %s %s\n",argv[1],argv[2],argv[3]);
    if(argc>=4)
    {int i;
    printf("%s",argv[3]);
    for(i=0;i<strlen(argv[3]);i++)
    {printf("%c\n",argv[3][i]);
      if(argv[3][i]=='f'||argv[3][i]=='b'||argv[3][i]=='r'||argv[3][i]=='l'||argv[3][i]=='s')
      {sprintf(hello,"%c\n",argv[3][i]);
       printf("%s sent\n",hello);
    send(sock , hello , strlen(hello) , 0 );
    delay(DEL);}
    }}
   strcpy(hello,"s \n");
        send(sock , hello , strlen(hello) , 0 );
    printf("%s sent\n",hello);
    delay(DEL); 
    //valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
