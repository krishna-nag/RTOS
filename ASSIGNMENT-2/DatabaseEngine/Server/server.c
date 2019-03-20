// Server progrm for the Database engine.

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>

#define PORT 8080 
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char sendbuf[1024]; 
    int debug=0; 
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,&opt, sizeof(int))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 6) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    while(1){
      // Whenever there is a new request for a socket, fork, so that a seperate dedicated process can handle that client.
     if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
            { 
                perror("accept"); 
                exit(EXIT_FAILURE); 
            }
     if (fork()==0){
            //Keep looking for any message from client. When a message comes, it is the filename.
            //Open the file, and send the data from the file in chunks of 1024 bits.
            while((valread=read( new_socket , buffer, 1024))){
              int fd = open(buffer, O_RDONLY);
              if (fd == -1) {
              strcpy(sendbuf, "Data not found\0");
              send(new_socket , sendbuf , strlen(sendbuf) , 0 );
              if(debug)printf("File not found\n");
              }
              if(debug)printf("replying to process A\n");

              
              int bytes=0;
              while((bytes = read(fd, sendbuf+1,1023 )) > 0){
              sendbuf[0]=0;
              if(bytes <1023){ 
                //Last packet
                sendbuf[0] = 1;
                (sendbuf+1)[bytes]='\0';
                    }
              send(new_socket , sendbuf , strlen(sendbuf) , 0 );


            }
            close(fd);
    }
    
    }
  }
    
    
    // valread = read( new_socket , buffer, 1024); 
    // printf("%s\n",buffer ); 
    // send(new_socket , hello , strlen(hello) , 0 ); 
    // printf("Hello message sent\n"); 
    return 0; 
} 