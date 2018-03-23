struct job
{
  char name[20];
  int burstTime;
  int priority;
  time_t arrival;
  time_t end;
};

void display_run(struct job job_queue[],int count,char policy_now[]);
void display_list(struct job job_queue[],int count,char policy_now[]);
void display_result(struct job result_queue[],int result_count);