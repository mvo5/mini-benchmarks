/*
 * build with:
 *   cc -o benchi benchi.c -lrt
 *
 */
#include<stdio.h>
#include <stdlib.h>
#include<time.h>

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
         execve(cmd[0], cmd, NULL);
         exit(0);
      }
      waitpid(pid, &status, 0);
      i++;


      clock_gettime(CLOCK_MONOTONIC, &t2);
      delta = ( (t2.tv_sec + (t2.tv_nsec/1000000000.0)) -
                (t1.tv_sec + (t1.tv_nsec/1000000000.0)));
      if(delta > duration_in_seconds)
         return i;
   }
}

int main(int argc, char **argv)
{
   int i, count;
   printf("fork/exec/sh/true test\n");

   const char *cmd1[] = {"/bin/true", NULL};
   const char *cmd2[] = {"/bin/sh", "-c", "/bin/true", NULL};

   printf("cmd: %s\n", cmd1[0]);
   count = get_nr_fork_exec_per_sec(cmd1, 1);
   printf("fork/exec per sec: %i\n", count);

   printf("cmd: %s\n", cmd2[0]);
   count = get_nr_fork_exec_per_sec(cmd2, 1);
   printf("fork/exec per sec: %i\n", count);

}
