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
#include <sys/inotify.h>
#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))

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
    //Setting up inotify
    int length, i = 0;
    int n_fd;
    int n_wd;
    char n_buffer[BUF_LEN];

    n_fd = inotify_init();

    if (n_fd < 0) {
        perror("inotify_init");
    }

    n_wd = inotify_add_watch(n_fd, ".",IN_MODIFY);
    
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
printf("socket\n");
	    int files_read=0;
            while(1){
		      int i=0;
		      length = read(n_fd, n_buffer, BUF_LEN);

        		if (length < 0) {
            		perror("read");
        		}

                while (i < length) {

                    struct inotify_event *event =
                        (struct inotify_event *) &n_buffer[i];
                    if (event->len) {
printf("%s\n",event->name);
                        if(strcmp(event->name,"sensor1.txt")==0) files_read=1;
                        if(strcmp(event->name,"sensor2.txt")==0 && files_read==1) files_read=2;
			if(strcmp(event->name,"sensor3.txt")==0 && files_read==2) files_read=3;
			if(strcmp(event->name,"sensor4.txt")==0 && files_read==3) files_read=4;
			if(strcmp(event->name,"sensor5.txt")==0 && files_read==4) files_read=5;
printf("%d\n",files_read);
                    }
                    i += EVENT_SIZE + event->len;
		    if(files_read==5){
			usleep(100000);
			//printf("%d\n",files_read);
			time_t clk = time(NULL);
			char timestamp_str[30]; 
			sprintf(timestamp_str, "%s", ctime(&clk));
			timestamp_str[strlen(timestamp_str)-1]=',';
			strcpy(sendbuf,"\0");
			strcat(sendbuf,timestamp_str);
			//printf("%s\n",sendbuf);
			
			FILE * fp[5];
			char sen[1024];
			int sencount = 0;
			fp[0] = fopen("sensor1.txt", "r");
			fp[1] = fopen("sensor2.txt", "r");
			fp[2] = fopen("sensor3.txt", "r");
			fp[3] = fopen("sensor4.txt", "r");
			fp[4] = fopen("sensor5.txt", "r");
				
			// Create a string to send
			int j = 0;
			for(j = 0; j < 5; j++){
				char ch;
				while((sen[sencount++] = fgetc(fp[j])) != EOF);
				sen[sencount-1] = ',';
				fclose(fp[j]);
			}
			sen[sencount++] = '\n';
			sen[sencount++] = '\0';
			strcat(sendbuf,sen);
			printf("%s",sendbuf);
			files_read=0;
			send(new_socket , sendbuf , strlen(sendbuf) , 0 );
		
			
		}
                }

		
    		
    		
        }

    }
    
    }
  }




































