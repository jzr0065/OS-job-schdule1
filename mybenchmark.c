
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>


int convert(char *string)
{
  int length = strlen(string);
  int number = 0;
  for(int i = 0; i < length; i++)
  {
    number = number * 10 + (string[i] - '0');
  } 
  return number;
}

int main(int argc, char *argv[] )
{
  int time;
  char *tem;
  //printf("argc number %d\n",argc);
  if(argc == 2)
  {
    tem = argv[1];
    time =convert(tem);
    //printf("%d\n",time);
  }
  //printf("A process is running ...\n");
  /* Print all the arguments passed from the commandline */
  sleep(time);
  //printf("process finished\n");

  return 0;
}
