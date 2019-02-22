/* Filename: msgq_recv.c */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define PERMS 0644
struct my_msgbuf {
   long mtype;
   char mtext[200];
};

int main(void) {
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
      exit(1);
   }
   printf("message queue RCV: ready to receive messages.\n");

//////////////// Msg queue process 1 is used to transmit messages to process 1 /////////////////////////////
  if ((key_p1 = ftok("../pA.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msq_p1 = msgget(key_p1, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      exit(1);
   }
   printf("message queue PA: ready\n");

//////////////// Msg queue process 2 is used to transmit messages to process 2 /////////////////////////////
  if ((key_p2 = ftok("../pB.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msq_p2 = msgget(key_p2, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      exit(1);
   }
   printf("message queue pB: ready \n");

//////////////// Msg queue process 3 is used to transmit messages to process 3 /////////////////////////////
  if ((key_p3 = ftok("../pC.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msq_p3 = msgget(key_p3, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      exit(1);
   }
   printf("message queue pC: ready \n");
//////////////// Msg queue process 4 is used to transmit messages to process 4 /////////////////////////////
  if ((key_p4 = ftok("../pD.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msq_p4 = msgget(key_p4, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      exit(1);
   }
   printf("message queue pD: ready \n");
//////////////// Msg queue process 5 is used to transmit messages to process 5 /////////////////////////////
  if ((key_p5 = ftok("../pE.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msq_p5 = msgget(key_p5, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      exit(1);
   }
   printf("message queue pE: ready \n");



   
   for(;;) { 
//Keeps checking for messages in the msq_rec
      if (msgrcv(msq_rec, &buf, sizeof(buf.mtext), 0,1) == -1) {
         perror("msgrcv");
         exit(1);
      }
      //printf("recvd: \"%s\"\n", buf.mtext);
      //toend = strcmp(buf.mtext,"end");
      //if (toend == 0)
      //break;
      if(strlen(buf.mtext)!=1)
	     continue;
      char rcv=buf.mtext[0];
      if(rcv>='A' && rcv<='Z')
	     sendbuf.mtext[0]=rcv+32;
      else if(rcv>='a' && rcv <='z')
	     sendbuf.mtext[0]=rcv-32;
      else
   	   sendbuf.mtext[0]=rcv;
      sendbuf.mtext[1]='\0';
      sendbuf.mtype=1; //doesn't matter

      if(buf.mtype==1){ //Reply to process A
	     if (msgsnd(msq_p1, &sendbuf, 2, 0) == -1) 
        perror("msgsnd");
      }
      else if(buf.mtype==2){//Reply to process B
	sendbuf.mtype=2;
//printf("recvd: \"%s come till here\"\n", buf.mtext);
         if (msgsnd(msq_p2, &sendbuf, 2, 0) == -1) 
         perror("msgsnd");
      }
      else if(buf.mtype==3){ //Reply to process C
	sendbuf.mtype=3;         
	if (msgsnd(msq_p3, &sendbuf, 2, 0) == -1) 
         perror("msgsnd");
      }
      else if(buf.mtype==4){//Reply to process D
	sendbuf.mtype=4;         
	if (msgsnd(msq_p4, &sendbuf, 2, 0) == -1) 
         perror("msgsnd");
      }
      else if(buf.mtype==5){ //Reply to process E
	sendbuf.mtype=5;         
	if (msgsnd(msq_p5, &sendbuf, 2, 0) == -1) 
         perror("msgsnd");
      }
      else{
         continue;
      }

   }
   printf("message queue: done receiving messages.\n");
   return 0;
}
