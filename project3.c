#include <pthread.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <string.h>
#include <time.h>
#include "help.h"
#include "parser.h"
#include "displayFuncs.h"
#include "scheFunc.h"

#define BUFFER_SIZE 10
#define NUM_THREADS 2

struct job job_queue[10],result_queue[100];

//initialize the global value which includes the number in job_queue now and the number in result_queue now.
pthread_mutex_t queue_mutex;
pthread_cond_t queue_notFull_cv;
pthread_cond_t queue_notEmpty_cv;
char policy_now[20] = {"fcfs"};
int count = 0;
int stop = 0;
int result_count = 0;

//scheduler function
void *producer(void *a) {
  int i;
  struct job j;
  struct com *para = a;
  //if the command is run
  if(strcmp(para->operation,"run") == 0)
  {
    pthread_mutex_lock(&queue_mutex);
    if(count == BUFFER_SIZE) {
      //printf("Producer: buffer has been full\n");
      //printf("Producer:waiting for notFull signal\n");
      pthread_cond_wait(&queue_notFull_cv,&queue_mutex); // first relase the lock and acquire the lock when receive the signal
      //printf("Producer:receive the notFull signal\n");
    }
    if(count == 0)
    {
      pthread_cond_signal(&queue_notEmpty_cv);
     // printf("Producer:notEmpty signal just sent\n");
    }
    j.burstTime = para->job_time;
    j.priority = para->job_pri;
    strcpy(j.name,para->job_name);
    j.arrival = time(NULL);
    job_queue[count] = j;
    count++;
    schedule(policy_now,job_queue,count); 
    pthread_mutex_unlock(&queue_mutex);
  }
  //if command is fcfs
  else if(strcmp(para->operation,"fcfs") == 0)
  {
    pthread_mutex_lock(&queue_mutex);
    if(strcmp("fcfs",policy_now))
    {
      strcpy(policy_now,"fcfs");
      schedule("fcfs",job_queue,count);
    }
    pthread_mutex_unlock(&queue_mutex);
  }
  //if command is sjf
  else if(strcmp(para->operation,"sjf") == 0)
  {
    pthread_mutex_lock(&queue_mutex);
    if(strcmp("sjf",policy_now))
    {
      strcpy(policy_now,"sjf");
      schedule("sjf",job_queue,count);
    }
    pthread_mutex_unlock(&queue_mutex);
  }
  //if command is priority
  else if(strcmp(para->operation,"priority") == 0)
  {
    pthread_mutex_lock(&queue_mutex);
    if(strcmp("priority",policy_now))
    {
      strcpy(policy_now,"priority");
      schedule("priority",job_queue,count);
    }
    pthread_mutex_unlock(&queue_mutex);
  }
  //if command is test
  else if(strcmp(para->operation,"test") == 0)
  {

    //batch submit
    count = 0;
    result_count = 0;
    strcpy(policy_now,para->policy);
    int tem ;
    //using the loop to create the job and sleep(1) imply the arriving rate is 1.0/second
    for(int i = 0; i < para->num_jobs; i++)
    { 
      pthread_mutex_lock(&queue_mutex);
      srand(i);
      tem = rand();
      j.burstTime = tem%(para->max_cpu - para->min_cpu) + para->min_cpu;
      //printf("%d\n",j.burstTime);
      j.priority = (tem%para->job_pri);
      strcpy(j.name,para->job_name);
      j.arrival = time(NULL);
      job_queue[count] = j;
      count++;
      if(i == 0)
      {
        pthread_cond_signal(&queue_notEmpty_cv);
        
      }
      schedule(policy_now,job_queue,count);
      pthread_mutex_unlock(&queue_mutex);
      sleep(1);
    }
    //after the schedule, rename the last 1 as 'LAST1', so , the program will display the result automatically without "quit" command.
    strcpy(job_queue[count-1].name,"LAST1");
  }
}

void *consumer(void *t) {

  while(stop == 0)
  {
    pid_t pid;
    char *my_args[5]; 
    char tem[10];
    struct job j1;
    pthread_mutex_lock(&queue_mutex);
    if (count == 0) {
     // printf("Consumer: the buffer is empty, waiting for not empty signal\n");
      pthread_cond_wait(&queue_notEmpty_cv, &queue_mutex);
      //printf("Consumer:receive the  notEmpty signal\n");
    }
    //printf("consumer: %d\n",job_queue[0].burstTime);
    j1 = job_queue[0];
    sprintf(tem,"%d",j1.burstTime);
    //printf("the tem :  %s\n",tem);
    my_args[0] = j1.name;
    my_args[1] = tem;
    my_args[2] = NULL;
    if(count == BUFFER_SIZE)
    {
      //printf("Consumer: buffer not full");
      pthread_cond_signal(&queue_notFull_cv);
      //printf("Consumer: notFull signal just sent");
    }
    //consumer_schedule();
    pthread_mutex_unlock(&queue_mutex);
    //create the child process
    switch ((pid = fork()))
    {
      case -1:
        /* Fork() has failed */
        perror("fork");
        break;
      case 0:
        /* This is processed by the child */
        execv(j1.name, my_args);
        puts("Uh oh! plz restart the program and input the right name");
        exit(EXIT_FAILURE);
        break;
      default:
        /* This is processed by the parent */
        //waiting for the end of the execution
        wait();
        pthread_mutex_lock(&queue_mutex);
        //grab the result and put it into the result queue
        count--;
        result_queue[result_count] = job_queue[0];
        result_queue[result_count].end = time(NULL);
        result_count++;
        if(strcmp(job_queue[0].name,"LAST1") == 0)
        {
          display_result(result_queue,result_count);
          result_count = 0;
        }
        struct job temJob;
        //cross off the job_queue[0], which means running job.
        if(count == 0)
        {
          job_queue[0] = temJob;
        }
        else
        {
          for(int j = 0 ; j< count + 1; j++)
          {
            job_queue[j] = job_queue[j+1];
          }
          job_queue[count+1] = temJob;
        }
        pthread_mutex_unlock(&queue_mutex);
        //printf("job has been move to result queue\n");
        break;
    }
    
    
  }
}



int main(int argc, char *argv[])
{ 
  printf("welcome to Jiaxiang Ren's batch job scheduler Version 1.0, Type 'Help' to find more about Aubatch commands\n");

  //initialize the threads
  int i;
  long t1=1, t2=2;
  pthread_t threads[2];
  pthread_attr_t attr;

   // Initialize mutex and condition variable objects 
  pthread_mutex_init(&queue_mutex, NULL);
  pthread_cond_init(&queue_notFull_cv, NULL);
  pthread_cond_init(&queue_notEmpty_cv,NULL);
   // For portability, explicitly create threads in a joinable state 
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&threads[1], &attr, consumer,(void *)t2);
  char *command;
  size_t size = 64;
  command = (char*) malloc(size*sizeof(char));
  while(1)
  { 
    printf(">");
    getline(&command,&size,stdin);
    struct com a;
    // printf("command line: %s\n",command)

    a = parser(command);
    //printf("%s\n",a.job_name);
    if(strcmp(a.operation,"help") == 0)
    {
       help();
    }
    else if (strcmp(a.operation,"run") == 0)
    {
      
      pthread_create(&threads[0], &attr, producer,&a);
      /* Wait for all threads to complete */
      pthread_join(threads[0], NULL);
      printf("job submitted\n");
      display_run(job_queue,count,policy_now);

      
    }
    else if(strcmp(a.operation,"test") == 0)
    {   
        printf("Please wait for result, you can type command 'list' to check the running status\n");
        pthread_create(&threads[0], &attr, producer,&a);
      /* Wait for all threads to complete */
        pthread_join(threads[0], NULL);
        //printf("test function\n");
    }
    else if(strcmp(a.operation,"list") == 0)
    {   
        display_list(job_queue,count,policy_now);
    }
    else if(strcmp(a.operation,"fcfs") == 0)
    {
        pthread_create(&threads[0], &attr, producer,&a);
        /* Wait for all threads to complete */
        pthread_join(threads[0], NULL);
        printf("Policy has changed to %s,all the %d jobs has been resheduled\n",policy_now,(count-1));
    }
    else if(strcmp(a.operation,"sjf") == 0)
    {
        pthread_create(&threads[0], &attr, producer,&a);
        /* Wait for all threads to complete */
        pthread_join(threads[0], NULL);
        printf("Policy has changed to %s,all the %d jobs has been resheduled\n",policy_now,(count-1));
    }
    else if(strcmp(a.operation,"priority") == 0)
    {   
        pthread_create(&threads[0], &attr, producer,&a);
        /* Wait for all threads to complete */
        pthread_join(threads[0], NULL);
        printf("Policy has changed to %s,all the %d jobs has been resheduled\n",policy_now,(count-1));
    }
    else if(strcmp(a.operation,"quit") == 0)
    {
      /* Clean up and exit */
      
      display_result(result_queue,result_count);
      stop = 1;
      break;
      pthread_cond_signal(&queue_notEmpty_cv);
      pthread_attr_destroy(&attr);
      pthread_mutex_destroy(&queue_mutex);
      pthread_cond_destroy(&queue_notFull_cv);
      pthread_cond_destroy(&queue_notEmpty_cv);
      pthread_exit (NULL);

    }
    else
    {
      printf("input error,please use the help func to check how to use it\n");
    }
  }

}