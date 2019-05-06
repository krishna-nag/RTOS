// Server side implementation of UDP client-server model 
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

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>

#define HEADER1 12
#define PORT     8080 
#define TOTAL 1024
#define MAXLINE 1024-HEADER1 
#define BUFSIZE 1024-HEADER1

int src;
int cp1;
int cp2;
int cnt;


int play_it(int sn,  pa_simple *s, char buf[TOTAL], int *error, int t){
    if (src==0 && cnt<t){
        if (sn>cp1){
            return pa_simple_write(s, buf+12, BUFSIZE, error);
            cp1=cp1+1;
            cnt=cnt+1;        
        }
    }
    else if(src==1 && cnt<t){
        if (sn>cp2){
            return pa_simple_write(s, buf+12, BUFSIZE, error);
            cp2=cp2+1;
            cnt=cnt+1;        
        }
    }
    else if (cnt==t){
        cnt=0;
        src=1;
    }
    // return pa_simple_write(s, buf+12, BUFSIZE, error);
}

int toint(char str[])
{
    int len = strlen(str);
    int i, num = 0;
 
    for (i = 0; i < len; i++)
    {
        num = num + ((str[len - (i + 1)] - '0') * pow(10, i));
    }
 
   return num;
}
  
// Driver code 
int main(int argc, char*argv[]) { 
    int len;
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from server"; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    int fd = open("file_rcvd",O_WRONLY);

    /* The Sample format to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };

    pa_simple *s = NULL;
    int ret = 1;
    int error;

      
    while(1){

    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
    cp1 = 0;
    cp2 = 0;

    for (;;) {
        uint8_t buf[TOTAL];
        ssize_t r;

#if 0
        pa_usec_t latency;

        if ((latency = pa_simple_get_latency(s, &error)) == (pa_usec_t) -1) {
            fprintf(stderr, __FILE__": pa_simple_get_latency() failed: %s\n", pa_strerror(error));
            goto finish;
        }

        fprintf(stderr, "%0.0f usec    \r", (float)latency);
#endif

        /* Read some data ... */
        if ((r = recvfrom(sockfd, (char *)buf, (size_t)BUFSIZE,  MSG_WAITALL, (struct sockaddr *) &cliaddr, &len)) <= 0) {
            if (r == 0) /* EOF */
                break;

            fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
            goto finish;
        }
        // char buf2[TOTAL];
        char buf2[BUFSIZE];
        int i;
        char final[HEADER1];
        char src_id[4];
        int seq_no;
        char str_seq[4];
        int k;
        memset(src_id, 0, sizeof(src_id));
        memset(str_seq, 0, sizeof(str_seq));
        memset(final, 0, sizeof(final));
        for(i=0;i<HEADER1;i++){
            final[i] = buf[i];
            if(i<6 && i>=3){
                src_id[i-3] = buf[i];
            }
        }
        for(i=0;i<3;i++){
            str_seq[i]=final[i];
        }
        cnt=0;

        printf("final, header: %s\n",final);
        printf("src_id, header: %s\n",src_id);
        printf("seq_str, header: %s\n",str_seq);
        seq_no = toint(str_seq);
        printf("seq_no, header: %d\n",seq_no);

        if (src_id == "jok")
            src=0;
        else if (src_id == "jol")
            src=1;

        /* ... and play it */
        if (play_it(seq_no,s,buf,&error,5) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
            goto finish;
        }
        memset(buf, 0, sizeof(buf));
        memset(buf2, 0, sizeof(buf2));

    }

    /* Make sure that every single sample was played */
    if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        goto finish;
    }
    ret = 0;

finish:

    if (s)
        pa_simple_free(s);

    return ret;

    }
    return 0; 
}