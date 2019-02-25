/* Filename: msgq_recv.c */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define PERMS 0644
struct my_msgbuf {
   long mtype;
   char mtext[201];
};
int msq_rec;
   int msq_p1;
   int msq_p2;
   int msq_p3;
   int msq_p4;
   int msq_p5;
void signalhandle(int signo) // If we press ctrl+C 
{
	if (signo == SIGINT){
		printf("Ctrl+C\n");
		msgctl(msq_rec, IPC_RMID, NULL);
		msgctl(msq_p1, IPC_RMID, NULL);
		msgctl(msq_p2, IPC_RMID, NULL);
		msgctl(msq_p3, IPC_RMID, NULL);
		msgctl(msq_p4, IPC_RMID, NULL);
		msgctl(msq_p5, IPC_RMID, NULL);
    		exit(0);
	}
}
int main(void) {
   if (signal(SIGINT, signalhandle) == SIG_ERR)
   printf("\ncan't catch SIGINT\n");
   int debug=0;
   struct my_msgbuf buf;
   struct my_msgbuf sendbuf;
   int msq_rec;
   int msq_p1;
   int msq_p2;
   int msq_p3;
   int msq_p4;
   int msq_p5;
   int toend;
   key_t key_rec;
   key_t key_p1;
   key_t key_p2;
   key_t key_p3;
   key_t key_p4;
   key_t key_p5;
//////////////// Msg queue server receives requests from all processes /////////////////////////////
   
if ((key_rec = ftok("../server.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msq_rec = msgget(key_rec, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      //exit(1);
   }

   //printf("message queue RCV: ready to receive messages.\n");

//////////////// Msg queue process 1 is used to transmit messages to process 1 /////////////////////////////
  if ((key_p1 = ftok("../pA.txt", 'B')) == -1) {
      perror("ftok");
      //exit(1);
   }
   
   if ((msq_p1 = msgget(key_p1, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      //exit(1);
   }
  //printf("message queue PA: ready\n");

//////////////// Msg queue process 2 is used to transmit messages to process 2 /////////////////////////////
  if ((key_p2 = ftok("../pB.txt", 'B')) == -1) {
      perror("ftok");
      //exit(1);
   }
   
   if ((msq_p2 = msgget(key_p2, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      //exit(1);
   }
   //printf("message queue pB: ready \n");

//////////////// Msg queue process 3 is used to transmit messages to process 3 /////////////////////////////
  if ((key_p3 = ftok("../pC.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msq_p3 = msgget(key_p3, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      //exit(1);
   }
   //printf("message queue pC: ready \n");
//////////////// Msg queue process 4 is used to transmit messages to process 4 /////////////////////////////
  if ((key_p4 = ftok("../pD.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msq_p4 = msgget(key_p4, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      //exit(1);
   }
   //printf("message queue pD: ready \n");
//////////////// Msg queue process 5 is used to transmit messages to process 5 /////////////////////////////
  if ((key_p5 = ftok("../pE.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msq_p5 = msgget(key_p5, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      //exit(1);
   }
   //if (debug) printf("message queue pE: ready \n");

struct timeval stop, start;


   for(;;) { 
//Keeps checking for messages in the msq_rec
      if (msgrcv(msq_rec, &buf, sizeof(buf.mtext), 0,1) == -1) {
         perror("msgrcv");
	msgctl(msq_rec, IPC_RMID, NULL);
         exit(1);
      }
	if(debug)printf("File requested is %s\n",buf.mtext);
	gettimeofday(&start, NULL);
      int fd = open(buf.mtext, O_RDONLY);
      if (fd == -1) {
	strcpy(sendbuf.mtext+1, "Data not found");
	sendbuf.mtext[0] = 1;
	if(buf.mtype==1)
	msgsnd(msq_p1, &sendbuf, sizeof(sendbuf), 0);
	if(buf.mtype==2)
	msgsnd(msq_p2, &sendbuf, sizeof(sendbuf), 0);
	if(buf.mtype==3)
	msgsnd(msq_p3, &sendbuf, sizeof(sendbuf), 0);
	if(buf.mtype==4)
	msgsnd(msq_p4, &sendbuf, sizeof(sendbuf), 0);
	if(buf.mtype==5)
	msgsnd(msq_p5, &sendbuf, sizeof(sendbuf), 0);
	
	if(debug)printf("File not found\n");
	continue;
		}
      
      
      

      if(buf.mtype==1){ //Reply to process A
	if(debug)printf("replying to process A\n");
	sendbuf.mtype=1;	
	int bytes=0;
	while((bytes = read(fd, sendbuf.mtext+1, 200)) > 0){
	sendbuf.mtext[0]=0;
	if(bytes <200){ 
		sendbuf.mtext[0] = 1;
		for(int i = bytes; i < 200; i++) (sendbuf.mtext+1)[i] = '\0'; 
				}
	if (msgsnd(msq_p1, &sendbuf, sizeof(sendbuf.mtext), 0) == -1) 
        	perror("msgsnd");	
	}
	     
	gettimeofday(&stop, NULL);
	printf("%lu,\n",stop.tv_usec - start.tv_usec);
      }
      else if(buf.mtype==2){//Reply to process B
	sendbuf.mtype=2;
	if(debug)printf("replying to process B\n");
        int bytes=0;
	while((bytes = read(fd, sendbuf.mtext+1, 200)) > 0){
	sendbuf.mtext[0]=0;
	if(bytes < 200){ 
		sendbuf.mtext[0] = 1;
		for(int i = bytes; i < 200; i++) (sendbuf.mtext+1)[i] = '\0'; 
				}
	if (msgsnd(msq_p2, &sendbuf, sizeof(sendbuf.mtext), 0) == -1) 
        	perror("msgsnd");	
	}
	     
	gettimeofday(&stop, NULL);
	printf("%lu,\n",stop.tv_usec - start.tv_usec);
      }
      else if(buf.mtype==3){ //Reply to process C
	sendbuf.mtype=3;         
	int bytes=0;
	if(debug)printf("replying to process C\n");
	while((bytes = read(fd, sendbuf.mtext+1, 200)) > 0){
	sendbuf.mtext[0]=0;
	if(bytes < 200){ 
		sendbuf.mtext[0] = 1;
		for(int i = bytes; i < 200; i++) (sendbuf.mtext+1)[i] = '\0'; 
				}
	if (msgsnd(msq_p3, &sendbuf, sizeof(sendbuf.mtext), 0) == -1) 
        	perror("msgsnd");	
	}
	     
	gettimeofday(&stop, NULL);
	printf("%lu,\n",stop.tv_usec - start.tv_usec);
      }
      else if(buf.mtype==4){//Reply to process D
	sendbuf.mtype=4;         
	int bytes=0;
	if(debug)printf("replying to process D\n");
	while((bytes = read(fd, sendbuf.mtext+1, 200)) > 0){
	sendbuf.mtext[0]=0;
	if(debug) printf("bytes=%d\n",bytes);
	if(bytes < 200){ 
		sendbuf.mtext[0] = 1;
		for(int i = bytes; i < 200; i++) (sendbuf.mtext+1)[i] = '\0';
		if(debug)printf("replied %d\n",bytes);
				}
	if (msgsnd(msq_p4, &sendbuf, sizeof(sendbuf.mtext), 0) == -1) 
        	perror("msgsnd");
	if(debug)printf("sent %d\n",bytes); 	
	}
	     
	gettimeofday(&stop, NULL);
	printf("%lu,\n",stop.tv_usec - start.tv_usec);
      }
      else if(buf.mtype==5){ //Reply to process E
	sendbuf.mtype=5;         
	int bytes=0;
	if(debug)printf("replying to process E\n");
	while((bytes = read(fd, sendbuf.mtext+1, 200)) > 0){
	sendbuf.mtext[0]=0;
	if(bytes < 200){ 
		sendbuf.mtext[0] = 1;
		for(int i = bytes; i < 200; i++) (sendbuf.mtext+1)[i] = '\0'; 
				}
	if (msgsnd(msq_p5, &sendbuf, sizeof(sendbuf.mtext), 0) == -1) 
        	perror("msgsnd");	
	}
	     
	gettimeofday(&stop, NULL);
	printf("%lu,\n",stop.tv_usec - start.tv_usec);
      }
      else{
         continue;
      }

   }
   printf("message queue: done receiving messages.\n");
   return 0;
}
