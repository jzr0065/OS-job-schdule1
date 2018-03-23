#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "parser.h"
#include <stdlib.h>


//calculate the number of variables
int count_var(char *s)
{
  int count = 0;
  char *ptr = s;
  while((ptr = strchr(ptr, ' ')) != NULL) {
      count++;
      ptr++;
  }
  return count+1;
}


//using the number of variables to check the operation and assign the corresponding value to data structure.
struct com parser(char *command)
{
  char c;
  struct com a;
  a.count = count_var(command);
  // printf("Count: %d\n",a.count);
  if(a.count == 4)
  { 
    // printf("run function\n");
    sscanf(command,"%s %s %d %d",a.operation,a.job_name,&a.job_time,&a.job_pri);
    // printf("%s %s %d %d\n",a.operation,a.job_name,a.job_time,a.job_pri);
  }
  else if(a.count == 3)
  { 
    // printf("run function\n");
    sscanf(command,"%s %s %d",a.operation,a.job_name,&a.job_time);
    // printf("%s %s %d\n",a.operation,a.job_name,a.job_time);
  }
  else if(a.count == 7)
  { 
    // printf("test function\n");
    sscanf(command,"%s %s %s %d %d %d %d",a.operation,a.job_name,a.policy,&a.num_jobs,&a.job_pri,&a.min_cpu,&a.max_cpu);
    // printf("%s %s %s %d %d %d %d\n",a.operation,a.benchmark,a.policy,a.num_jobs,a.job_pri,a.min_cpu,a.max_cpu);
  }
  else if(a.count == 1)
  {
    // printf("help function\n");
    sscanf(command,"%s",a.operation);
  }
  return a;
}