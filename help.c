#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "help.h"
#include <stdlib.h>


void help()
{
  printf("run <job> <time> <pri>: \nsubmit a job named <job>,\nexecution time is <time>,\npriority is <pri>");
  printf("list: display the job status\n");
  printf("fsfs: change the scheduling policy to FCFS\n");
  printf("sjf: change the scheduling policy to SJF\n");
  printf("test <benchmark> <policy> <num_of_jobs> <priority_levels> <min_CPU_time> <max_CPU_time>\n");
  printf("quit: Aubatch\n");
}