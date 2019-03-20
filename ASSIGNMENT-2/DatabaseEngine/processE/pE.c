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
    char buffer[1024] = {0};
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
    strcpy(buffer,"E.txt\0");
    
    gettimeofday(&start, NULL);
    send(sock , buffer , strlen(buffer) , 0 ); 

    if(debug) printf("Requested\n");
    int clientfd = open(buffer , O_WRONLY | O_CREAT, PERMS);
    while((valread=read( sock , buffer, 1024))){
        if(buffer[0]==1){
           write(clientfd, buffer+1, 1023);break;} 
        
        write(clientfd,buffer+1,1023);
    }
    if(debug) printf("Received\n");
    gettimeofday(&stop, NULL);
    printf("%lu,\n",stop.tv_usec - start.tv_usec);
    close(clientfd);
    }
    return 0; 
} 
