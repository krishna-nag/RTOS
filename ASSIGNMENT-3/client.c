
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>

#define BUFSIZE 1024

 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#define PORT 8080 
#define PERMS 0644




int main(int argc, char*argv[]) {
    //socket stuff
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0};

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

    /* The sample type to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };
    pa_simple *s = NULL;
    int ret = 1;
    int error;
    int debug=0;
    if(debug) printf("started\n");
    /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stdout, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
    if(debug) printf("loop\n");
    for (;;) {
        uint8_t buf[BUFSIZE];
    //if(debug) printf("inside loop\n");
        /* Record some data ... */
        if (pa_simple_read(s, buf, sizeof(buf), &error) < 0) {
            fprintf(stdout, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }
	
	send(sock,buf,sizeof(buf),0);
        
    }

    ret = 0;

finish:

    if (s)
        pa_simple_free(s);

    return ret;
}