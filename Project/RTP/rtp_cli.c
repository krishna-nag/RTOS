
// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <time.h>

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>
  
#define HEADER1 12
#define PORT     8080 
#define TOTAL 1024
#define MAXLINE 1024-HEADER1 
#define BUFSIZE 1024-HEADER1
//#define IP_ADDRESS "172.16.132.111" // localhost 
// #define IP_ADDRESS "68.183.82.118" // localhost 
 #define IP_ADDRESS "172.16.145.71" // localhost 

struct sockaddr_in     servaddr; 
int sockfd; 
int seq_no;
// int src_id;

// https://www.sanfoundry.com/c-program-integer-to-string-vice-versa/

void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

/* A simple routine calling UNIX write() in a loop */
static ssize_t loop_write(int fd, void*data, size_t size) {
    printf("sockfd = %d\n",sockfd);
    ssize_t ret = 0;

    while (size > 0) {
        ssize_t r;

        char *hello = "Hello from client"; 

        // if ((r = write(fd, data, size)) < 0)
        if ((r = sendto(sockfd, (char *)data, size,MSG_CONFIRM, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0)
            return r;

        if (r == 0)
            break;

        ret += r;
        data = ( uint8_t*) data + r;
        size -= (size_t) r;
    }

    return ret;
}
  
// Driver code 
int main(int argc, char*argv[]) { 
    char buffer[MAXLINE]; 
    char *hello = "A paragraph is a self-contained unit of a discourse in writing dealing with a particular point or idea. A paragraph consists of one or more sentences. Though not required by the syntax of any language, paragraphs are usually an expected part of formal writing, used to organize longer prose. A paragraph is a self-contained unit of a discourse in writing dealing with a particular point or idea. A paragraph consists of one or more sentences. Though not required by the syntax of any language, paragraphs are usually an expected part of formal writing, used to organize longer prose. A paragraph is a self-contained unit of a discourse in writing dealing with a particular point or idea. A paragraph consists of one or more sentences. Though not required by the syntax of any language, paragraphs are usually an expected part of formal writing, used to organize longer prose. A paragraph is a self-contained unit of a discourse in writing dealing with a particular point or idea. A paragraph consists of456789123"; 
    // char *hello = "Hello from client"; 
//rec setup
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };
    pa_simple *s = NULL;
    int ret = 1;
    int error;
    seq_no = 0;

    int fd = open("rec_file",O_WRONLY);
  //rec_end
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    // servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
      
    int n, len; 

    /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
    int i;
    char buf2[TOTAL];
    for (i=0;i<10000000;i++) {
        uint8_t buf[TOTAL];
        char header[HEADER1] = "derHeader";
        char str[3];
        tostring(str, i%1000);
        printf("str=%s\n",str);
        // int stri;
        // stri = toint(str);
        // printf("stri=%d\n",stri);

        strcat(header, str); //sequence_number
        strcat(header, argv[1]); //source id

        strcat(header, "Header");

        printf("header = %s\n",header);

        memset(buf, 0, sizeof(buf));
        memset(buf2, 0, sizeof(buf2));

        int i;
    for(i=0;i<HEADER1;i++){
        // printf("header i=%d\n",i);
        buf[i] = header[i];
    }

        /* Record some data ... */
        if (pa_simple_read(s, buf+12, BUFSIZE, &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }

        printf("%s",buf2);
        int ans = loop_write(fd, buf, sizeof(buf));
        // printf("ans = %d\n",ans);
        if ( ans!= sizeof(buf)) {
            fprintf(stderr, __FILE__": write() failed: %s\n", strerror(errno));
            goto finish;
        }

    }

    ret = 0;

finish:

    if (s)
        pa_simple_free(s);
    // close(sockfd);

    return ret;
} 
