#include <pthread.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <string.h>
#include "displayFuncs.h"


//sort the array using sjf
void sjf(int start,struct job *job_queue,int count)
{
  for (int i = start; i < count-1; ++i) 
  {
    for (int j = i + 1; j < count; ++j)
    {
      if (job_queue[i].burstTime > job_queue[j].burstTime) 
      {   
          struct job tem;
          tem =  job_queue[i];
          job_queue[i] = job_queue[j];
          job_queue[j] = tem;
      }
    }
  }
}


//sort the array using the fcfs
void fcfs(int start,struct job *job_queue,int count)
{
  for (int i = start; i < count-1; ++i) 
  {
    for (int j = i + 1; j < count; ++j)
    {
      if (job_queue[i].arrival > job_queue[j].arrival) 
      {   
          struct job tem;
          tem =  job_queue[i];
          job_queue[i] = job_queue[j];
          job_queue[j] = tem;
      }
    }
  }
}


//sort the array using the priority
void priority(int start,struct job *job_queue,int count)
{
  for (int i = start; i < count-1; ++i) 
  {
    for (int j = i + 1; j < count; ++j)
    {
      if (job_queue[i].priority < job_queue[j].priority) 
      {   
          struct job tem;
          tem =  job_queue[i];
          job_queue[i] = job_queue[j];
          job_queue[j] = tem;
      }
    }
  }
}

//shedule controller.
void schedule(char *policy,struct job *job_queue,int count)
{
    
    if(count > 2)
    {
      if(strcmp(policy,"sjf") == 0)
        sjf(1,job_queue,count);
      if(strcmp(policy,"fcfs") == 0)
        fcfs(1,job_queue,count);
      if(strcmp(policy,"priority") == 0)
        priority(1,job_queue,count);
    }
}