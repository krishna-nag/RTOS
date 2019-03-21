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

void* sample_function(void* args){
	printf("thread begin\n");
	coroutine *c=(coroutine*)args;

	int age_limit=*((int*)(c->data)[2]);
	printf("waiting for sem %d\n",age_limit);
 	sem_wait((c->mutex));// for start
	printf("found sem\n");
 	while(1){
 		int age;
		age=*((int*) cread_thread(c));//yield
		printf("age=%d\n",age);
		if(age>=age_limit){
			int allowed=1;
			cwrite_thread(c,&allowed);
		}
		else{
			int allowed=0;
			cwrite_thread(c,&allowed);
		}
 	}

 }
 coroutine* create_coroutine(void* (*f)(void*),pthread_t* tr,int* age,sem_t* sem){

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
	data[2]=(void*) age;
	c->data=data;



    pthread_create(tr,NULL,f,c);
    return c;

 }

 int main(){

 pthread_t tr;
 int age=3;
 int debug=1;
 sem_t sems;
 sem_init(&sems, 0, 1); 
 sem_wait(&sems);
 if(debug) printf("debug has begun\n");
 coroutine* x=create_coroutine(sample_function,&tr,&age,&sems);
 if(debug) printf("coroutine obtained\n");
 sem_post(&sems);
 if(debug) printf("semaphore released\n");
 int new_age=1;
 sleep(5);
 cwrite(x,&new_age);
 int y=*((int*)cread(x));
printf("y=%d\n",y);


 
pthread_join(tr,NULL);
shmdt(x->data); 
sem_destroy(&sems);




 }