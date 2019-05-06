#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT     8080
#define MAXLINE 1024

// Driver code
int main() {
    char buffer[MAXLINE];
    char *hello = "Hello from server";
    int clients=2;
    struct sockaddr_in servaddr[clients], cliaddr[clients];
    int sockfd[clients];
    //int clients=2;
    // Creating socket file descriptor
    int sockfd2;
    int count=0,count_clients=0;

    for(count=0;count<clients;count++){
           memset(&servaddr[count], 0, sizeof(servaddr[count]));
           memset(&cliaddr[count], 0, sizeof(cliaddr[count]));

           // Filling server information
    servaddr[count].sin_family    = AF_INET; // IPv4
    servaddr[count].sin_addr.s_addr = INADDR_ANY;
    servaddr[count].sin_port = htons(PORT+count);
    }
    
    //memset(&cliaddr, 0, sizeof(cliaddr));

    
    // Bind the socket with the server address
 int len, n,f;   
for(count_clients=0;count_clients<clients;count_clients++){
  if ( (sockfd[count_clients] = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
  if ( bind(sockfd[count_clients], (const struct sockaddr *)&servaddr[count_clients],
            sizeof(servaddr[count_clients])) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Listening\n");
    n = recvfrom(sockfd[count_clients], (char *)buffer, MAXLINE,
                MSG_WAITALL, ( struct sockaddr *) &cliaddr[count_clients],
                &len);
    printf("%s\n",buffer);
    // sprintf(buffer, "%d", PORT+count_clients); 
    // //strcpy(buffer,itoa(PORT+count_clients));
    // sendto(sockfd[0], (char *)buffer, strlen(buffer),
    //         MSG_CONFIRM, (const struct sockaddr *) &cliaddr[0],
    //             len);
    // printf("Sent\n");
    // printf("%s\n",buffer);
}
// strcpy(buffer,"yep");
// for(count_clients=0;count_clients<clients;count_clients++){
// sendto(sockfd[0], (char *)buffer, strlen(buffer),
//             MSG_CONFIRM, (const struct sockaddr *) &cliaddr[client_id],
//                 len);
//     }
//   printf("Sent yeps\n");  
    int client_id;
    for(count_clients=0;count_clients<clients;count_clients++){

        n = recvfrom(sockfd[count_clients], (char *)buffer, MAXLINE,
                MSG_WAITALL, ( struct sockaddr *) &cliaddr[count_clients],
                &len);
        if(count_clients==0){
            client_id=0;
            printf("%d\n",client_id);
        }
        else{
            f=fork();
            if(f){
                client_id=count_clients;
                printf("%d\n",client_id);

                break;
            }
        }
        // buffer[n] = '\0';
        // printf("Client : %s\n", buffer);
        // char ipAddress[INET_ADDRSTRLEN];
        // inet_ntop(AF_INET, &(cliaddr[count_clients].sin_addr), ipAddress, INET_ADDRSTRLEN);
        // printf("inet: %s\n",ipAddress);
        // printf("socket: %d\n",sockfd2);
    }
    strcpy(buffer,"start");
    sendto(sockfd[client_id], (char *)buffer, strlen(buffer),
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr[client_id],
                len);
    while(1){
      n = recvfrom(sockfd[client_id], (char *)buffer, MAXLINE,
                MSG_WAITALL, ( struct sockaddr *) &cliaddr[client_id],
                &len);
      printf("recieved %s\n",buffer);
        int nc=0;
        for(nc=0;nc<clients;nc++){
        if(nc==client_id){
        }
        else{
        sendto(sockfd[nc], (char *)buffer, MAXLINE,
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr[nc],
            len);
        }
        }  
    }
//     printf("All clients connected\n");
//     int f;
//    for(count_clients=0;count_clients<clients-1;count_clients++){
//         f=fork();
//         if(f){
//         break;
//         count_clients++;
//         }
//     }
//     while(1){
//         n = recvfrom(sockfd[count_clients], (char *)buffer, MAXLINE,
//                 MSG_WAITALL, ( struct sockaddr *) &cliaddr[count_clients],
//                 &len);
//         int nc=0;
//         for(nc=0;nc<clients;nc++){
//         if(nc==count_clients){
//         }
//         else{
//         sendto(sockfd[nc], (char *)buffer, strlen(buffer),
//         MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
//             len);
//         }
//         }
//         }
// sendto(sockfd, (const char *)hello, strlen(hello),
//         MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
//             len);
    printf("Hello message sent.\n");
    return (0);
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                            1,1           All