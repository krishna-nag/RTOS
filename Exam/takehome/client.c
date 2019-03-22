// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#define PORT 8080 
#define PERMS 0644

   
int main(int argc, char const *argv[]) 
{ 
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024];
    int debug=0; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 


    struct timeval stop, start;




    int x=0;
    while(1){
     

    if(debug) printf("Requested\n");
    int clientfd = open("data.csv", O_WRONLY | O_CREAT, PERMS);
    while((valread=read( sock , buffer, 1024))){
        printf("%s",buffer);
        write(clientfd,buffer,strlen(buffer));
	strcpy(buffer,"\0");
    }
    
    close(clientfd);
    }
    return 0; 
} 

