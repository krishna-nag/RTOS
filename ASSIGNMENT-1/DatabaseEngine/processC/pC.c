
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
#include <fcntl.h>
#include <unistd.h>


#define PERMS 0644
struct my_msgbuf {
   long mtype;
   char mtext[201];
};

int main(void) {
   struct my_msgbuf buf;
   int msq_rec;
   int msq_p1;
   int len;
   key_t key_rec;
   key_t key_p1;
   int debug=0;
   
   if ((key_p1 = ftok("../pC.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msq_p1 = msgget(key_p1, PERMS | IPC_CREAT)) == -1) {
      perror("msgget");
      exit(1);
   }


   if ((key_rec = ftok("../server.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msq_rec = msgget(key_rec, PERMS | IPC_CREAT)) == -1) {
      perror("msgget");
      exit(1);
   }

   

   buf.mtype = 3; 
   
  struct timeval stop, start;
  


   time_t loop_start,cur;

   loop_start = time(NULL);
   cur=time(NULL);
   int x=0;
   while(x<10){
 	strcpy(buf.mtext,"C.txt\0");
	
	gettimeofday(&start, NULL);
        if (msgsnd(msq_rec, &buf, sizeof(buf.mtext), 0) == -1) /* +1 for '\0' */
      		perror("msgsnd");
	if(debug) printf("Requested %s\n",buf.mtext);
	int clientfd = open(buf.mtext, O_WRONLY | O_CREAT, PERMS);
	for(;;) { 
	//Keeps checking for messages in the msq_rec
        if (msgrcv(msq_p1, &buf, 201, 3, 0) == -1) {
         perror("msgrcv");
         exit(1);
        }
	if(buf.mtext[0]==1){
	write(clientfd, buf.mtext+1, 200);break;}
	write(clientfd,buf.mtext+1,200);


	}
	if(debug) printf("Received %s\n",buf.mtext);
	gettimeofday(&stop, NULL);
	printf("%lu,\n",stop.tv_usec - start.tv_usec);
	cur=time(NULL);
	close(clientfd);
x++;
   }



  
   return 0;
}
