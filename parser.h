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
struct com parser(char *command);