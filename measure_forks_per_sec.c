/*
 * build with:
 *   cc -o benchi benchi.c -lrt
 *
 */
#include<stdio.h>
#include <stdlib.h>
#include<time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int get_nr_fork_exec_per_sec(const char **cmd, int duration_in_seconds)
{
   struct timespec t1, t2;
   int pid, status,i;
   double delta;

   clock_gettime(CLOCK_MONOTONIC, &t1);
   i = 0;
   while(1)
   {
      pid = fork();
      if (pid == 0)
      {
         int fd = open("/dev/null", O_WRONLY);
         dup2(fd, 1);
         dup2(fd, 2);

         execve(cmd[0], cmd, NULL);
         exit(0);
      }
      waitpid(pid, &status, 0);
      i++;


      clock_gettime(CLOCK_MONOTONIC, &t2);
      delta = ( (t2.tv_sec + (t2.tv_nsec/1000000000.0)) -
                (t1.tv_sec + (t1.tv_nsec/1000000000.0)));
      if(delta > duration_in_seconds)
         return i/duration_in_seconds;
   }
}

int main(int argc, char **argv)
{
   int i, j, count;
   const char *cmds[][5] = {
      {"no-command", NULL, NULL},
      {"/bin/true", NULL, NULL},
      {"/bin/dash", "-c", "/bin/true", NULL},
      {"/bin/bash", "-c", "/bin/true", NULL},
      {"/usr/bin/curl", "file:/dev/null", NULL},
      {"/usr/bin/curl", "http://localhost", NULL},
      NULL, 
   };

   printf("fork/exec/sh/true test\n");
   for(i=0; cmds[i][0] != NULL; i++)
   {
      printf("cmd: ");
      for(j=0; cmds[i][j] != NULL; j++)
         printf("%s ", cmds[i][j]);
      printf("\n");
      count = get_nr_fork_exec_per_sec(cmds[i], 2);
      printf("fork/exec per sec: %i\n\n", count);
   }
}
