
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <time.h>


#define PERMS 0644
struct my_msgbuf {
   long mtype;
   char mtext[200];
};

int main(void) {
   struct my_msgbuf buf;
   int msq_rec;
   int msq_p1;
   int len;
   key_t key_rec;
   key_t key_p1;

   
   if ((key_p1 = ftok("../pE.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msq_p1 = msgget(key_p1, PERMS | IPC_CREAT)) == -1) {
      perror("msgget");
      exit(1);
   }
   printf("message queue pA: ready to send messages.\n");

   if ((key_rec = ftok("../server.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msq_rec = msgget(key_rec, PERMS | IPC_CREAT)) == -1) {
      perror("msgget");
      exit(1);
   }
   printf("message queue rec: ready\n");
   

   buf.mtype = 5; 
   
  

   time_t loop_start,cur;

   loop_start = time(NULL);
   cur=time(NULL);
   
   while(cur-loop_start <=30){
 	buf.mtext[0]='a';
	buf.mtext[1]='\0';
        if (msgsnd(msq_rec, &buf, 2, 0) == -1) /* +1 for '\0' */
      		perror("msgsnd");
	

	for(;;) { 
	//Keeps checking for messages in the msq_rec
        if (msgrcv(msq_p1, &buf, sizeof(buf.mtext), 5, 0) == -1) {
         perror("msgrcv");
         exit(1);
        }
        //printf("recvd: \"%s\"\n", buf.mtext);

	break;
	}
	cur=time(NULL);
   }



  
   return 0;
}
