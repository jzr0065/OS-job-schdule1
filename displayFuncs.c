
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <string.h>
#include <time.h>



//define the job strcut
struct job
{
  char name[20];
  int burstTime;
  int priority;
  time_t arrival;
  time_t end;
};


struct com
{
  char operation[10];
  char job_name[20];
  int  job_time;
  int  job_pri;
  int  min_cpu;
  int  max_cpu;
  int  num_jobs;
  char policy[10];
  char benchmark[20];
  int count;
};

//when submit sucessfully, display the waiting time and the number of jobs in queue.
void display_run(struct job job_queue[],int count,char policy_now[])
{
  printf("the job has been submitted\n");
  printf("total number jobs in queue: %d\n",count);
  int time = 0;
  if(count == 1)
  {
    printf("exepected waiting time: 0s\n");
  }
  else
  {
    for(int i = 0;i<count-1;i++)
    {
      time = time + job_queue[i].burstTime;
    }
    printf("exepected waiting time: %d\n",time);
  }
  printf("policy: %s\n",policy_now);
}

//when input "list", display the job_queue.
void display_list(struct job job_queue[],int count,char policy_now[])
{
  printf("Total number of jobs in the queue: %d\n", count);
  printf("Scheduling policy: %s\n",policy_now);
  printf("Name\t\tBusrt_time\t\tpri\tArrival_time\tProgress\n");
  for(int i = 0; i < count; i++)
  {
    struct tm * timeinfo;
    timeinfo = localtime ( &job_queue[i].arrival);
    if(i == 0)
      if(timeinfo->tm_sec > 9)
        printf("%s\t\t%d\t\t%d\t%d:%d:%d\tRun\n",job_queue[i].name,job_queue[i].burstTime,job_queue[i].priority,timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
      else
        printf("%s\t%d\t\t%d\t%d:%d:%d\t\tRun\n",job_queue[i].name,job_queue[i].burstTime,job_queue[i].priority,timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    else
    {
      printf("%s\t%d\t\t%d\t%d:%d:%d\n",job_queue[i].name,job_queue[i].burstTime,job_queue[i].priority,timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    }
  }
}


//when input "quit", display the job in result job_queue.
void display_result(struct job result_queue[],int result_count)
{
  double turnaround = 0;
  double burstTime = 0;
  if(result_count == 0)
  {
    printf("Program end\n");
    return ;
  }
  for(int i = 0;i < result_count; i++)
  {
    turnaround += difftime(result_queue[i].end,result_queue[i].arrival);
    burstTime += result_queue[i].burstTime;
  }
  printf("Total number jobs submitted:\t %d\n",result_count);
  printf("Average turnaround time:\t %.2f\n",turnaround/result_count);
  printf("Average BurstTime:\t\t %.2f\n",burstTime/result_count);
  printf("Average waiting time:\t\t %.2f\n",(turnaround-burstTime)/result_count);
  printf("Average Throughput:\t\t %.3f\n",(1/(turnaround/result_count)));
}