///////////////////////////////////////////////////////////////
/// Author Krshna Nagaraja ///////////////////////////////////
/// created at 5:30 PM, 20th March, 2019 ////////////////////
/// RTOS Exam question /////////////////////////////////////
///////////////////////////////////////////////////////////

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include <sys/shm.h> 
#include <sys/ipc.h> 

//Functions required are send and yield, and create_coroutine
//Coroutine creates a thread, with a pipe linked to main program
typedef struct {
    sem_t* mutex;
    void** data;
  } coroutine;

int data_available(coroutine *c){
return *((int*)(c->data)[0]);
}

void cwrite_thread(coroutine *c,void* x){
	(c->data)[2]=x;
	*((int*)(c->data)[1])=1;
}
void* cread_thread(coroutine *c){
while(!(*((int*)(c->data)[0])));
  *((int*)(c->data)[0])=0;
return (c->data)[2];
}

void cwrite(coroutine *c,void* x){
	(c->data)[2]=x;
	*((int*)(c->data)[0])=1;
}
void* cread(coroutine *c){
  while(!(*((int*)(c->data)[1])));
  *((int*)(c->data)[1])=0;
return (c->data)[2];
}

void* player(void* args){
	//printf("thread begin\n");
	coroutine *c=(coroutine*)args;

	//int age_limit=*((int*)(c->data)[2]);
	//printf("waiting for sem %d\n",age_limit);
 	sem_wait((c->mutex));// for start
	//printf("found sem\n");
 	while(1){
 		//int age;
		void* temp;
		temp=cread_thread(c);//yield
		//printf("age=%d\n",age);
		int n=0;
		n = rand() % 6 + 1;
		cwrite_thread(c,&n);
 	}

 }
 coroutine* create_coroutine(void* (*f)(void*),pthread_t* tr,sem_t* sem){

 	void* arg=(void*)malloc(sizeof(void*));
 	//coroutine x;
 	coroutine *c=(coroutine*)malloc(sizeof(coroutine));
 	
 	c->mutex=sem;
        // ftok to generate unique key 
        key_t key = ftok("shmfile",65); 
  
        // shmget returns an identifier in shmid 
        int shmid = shmget(key,1024,0666|IPC_CREAT); 

        // shmat to attach to shared memory 
	// data[0] would be used to indicate if something is changed in the data. Then the thread will read the data, and set it back to 0
	// data[1] would be used to indicate if something is changed in the data. The calling program will then read the data, and set it back to 0
        void **data = (void**) shmat(shmid,(void**)0,0);

	int *d1=(int*)malloc(sizeof(int));
	int *d2=(int*)malloc(sizeof(int));
	*d1=0;
	*d2=0;
	data[0]=(void*) d1;
	data[1]=(void*) d2;
	//data[2]=(void*) age;
	c->data=data;



    pthread_create(tr,NULL,f,c);
    return c;

 }
int play(coroutine* c){
cwrite(c,NULL);
return *((int*)cread(c));
}

int snake_ladder(int player,int cur_pos,int dice){
	int pos=cur_pos+dice;	
	if(pos>100){
	return cur_pos;	
	}
	if(pos==45){ //snake
	return 20;	
	}
	if(pos==15){ //snake
	return 5;	
	}
	if(pos==97){ //snake
	return 14;	
	}
	if(pos==73){ //snake
	return 25;	
	}
	if(pos==39){ //snake
	return 10;	
	}
	if(pos==3){ //ladder
	return 40;	
	}
	if(pos==21){ //ladder
	return 49;	
	}
	if(pos==26){ //ladder
	return 89;	
	}
	if(pos==43){ //ladder
	return 99;	
	}
	if(pos==16){ //ladder
	return 78;	
	}
}
void print_board(int p1,int p2,int p3, int p4){
int co=0;
system("clear");
for (int i=0;i<10;i++){

for(int j=0;j<10;j++){
co++;
if(co==p1) printf("1");
//else printf("");

if(co==p2) printf("2");
//else printf("");

if(co==p3) printf("3");
//else printf("");

if(co==p4) printf("4");
//else printf("");

printf("# ");

}
printf("\n");
}
printf("\n");
printf("\n");
}
 int main(){

 pthread_t tr1,tr2,tr3,tr4;
 int age=3;
 int debug=1;
 sem_t pl1,pl2,pl3,pl4;
 sem_init(&pl1, 0, 1); 
 sem_init(&pl2, 0, 1); 
 sem_init(&pl3, 0, 1); 
 sem_init(&pl4, 0, 1); 
 sem_wait(&pl1);
 sem_wait(&pl2);
 sem_wait(&pl3);
 sem_wait(&pl4);

//create players
coroutine* pc1=create_coroutine(player,&tr1,&pl1);
coroutine* pc2=create_coroutine(player,&tr2,&pl2);
coroutine* pc3=create_coroutine(player,&tr3,&pl3);
coroutine* pc4=create_coroutine(player,&tr4,&pl4);

//snakes and ladders would be a matrix plus some rules.
//Here the main function is the master, and there are 4 threads that are players
//Each thread will wait for its turn and roll the dice. It will return a random number between 1 to 6.
//The main program receives this, and it adjusts the postitons of the threads on the board
//Based on the rules, the thread will go through a snake or ladder.
// The game stops when any one wins

int **mat=(int**)malloc(10*sizeof(int*));
for(int i=0;i<10;i++){
mat[i]=(int*)malloc(10*sizeof(int));
for(int j=0;j<10;j++){
mat[i][j]=0;//initialisation
}
}
//Start Game
sem_post(&pl1);
sem_post(&pl2);
sem_post(&pl3);
sem_post(&pl4);


int p1=0,p2=0,p3=0,p4=0;
int win_flag=0;
while(1){

int x1=play(pc1);
p1=snake_ladder(1,p1,x1);
print_board(p1,p2,p3,p4);
if(p1==100)
win_flag=1;
sleep(1);

int x2=play(pc2);
p2=snake_ladder(2,p2,x2);
print_board(p1,p2,p3,p4);
if(p2==100)
win_flag=1;
sleep(1);

int x3=play(pc3);
p3=snake_ladder(3,p3,x3);
print_board(p1,p2,p3,p4);
if(p3==100)
win_flag=1;
sleep(3);

int x4=play(pc4);
p4=snake_ladder(4,p4,x4);
print_board(p1,p2,p3,p4);
if(p4==100)
win_flag=1;
sleep(1);
if(win_flag==1){
if(p1==100) printf("Player 1 wins\n");
if(p2==100) printf("Player 2 wins\n");
if(p3==100) printf("Player 3 wins\n");
if(p4==100) printf("Player 4 wins\n");
break;
}
}

pthread_join(tr1,NULL);
pthread_join(tr2,NULL);
pthread_join(tr3,NULL);
pthread_join(tr4,NULL);
shmdt(pc1->data); 
shmdt(pc2->data); 
shmdt(pc3->data); 
shmdt(pc4->data); 
sem_destroy(&pl1);
sem_destroy(&pl2);
sem_destroy(&pl3);
sem_destroy(&pl4);


 }