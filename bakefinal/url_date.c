/************************************************************************
*      Experimenting with curl & pipe                                   *
*                                                                       *
*        To compile :                                                   *
*        $ cc -std=c99 -Wall -pedantic -Werror -o curl_test curl_test.c *
*        the fucntion url_date inputs the url into the commandline using*
*        execl and pipes the output of the curl function to the         *
*        parent process. the output returned to the parent process is   *
*        the last-modified date of the URL. Curl getdate is then used to*
*         return time in seconds since the epoch. If the URL does not   *
*        return a last-modified date then an error occurs. we recogniesd*
*        not all severs return a last-modified date and some return in  *
*        a different format.In this function we have specifically       *
*        decided to go with the last-modified server format date.       *
************************************************************************/

#include"header.h"

time_t url_date(char *Dstr)
{
  char *date_time = NULL;
  char *shell = getenv("SHELL"); /* Returns a pointer with necessary memory allocated */
  char *fshel = basename(shell); /* Gets file name from full path */
  if(!shell || !fshel) /* If either function returned a null pointer use default values*/
  {
    shell = strdup("/bin/bash");
    MALLOC_CHECK(shell)
    fshel = strdup("bash");
    MALLOC_CHECK(fshel)
  }

  
    char arg[BUFSIZ] = "curl --head "; // command to be entered into the command line
    char back_end[BUFSIZ] = " | grep -i Last-Modified"; // uses grep to search the output for last-Modified
    strcat(arg, Dstr);
    strcat(arg,back_end);


    int pipefd[2];
    if(pipe(pipefd) < 0)
    {
      fprintf(stderr, "bake : Fatal piping failure on checking of URL dependency \"%s\"\n",Dstr);
      exit(EXIT_FAILURE);
    }

    int status;
    int pid = fork();// forking creating child and parent process
    char buf[BUFSIZ];
    switch (pid)
    {
      case -1 :
        fprintf(stderr, "bake : Fatal forking failure on checking of URL dependency \"%s\"\n",Dstr);
        exit(EXIT_FAILURE);
        break;

      case 0 :// in the child process
        dup2(pipefd[1], STDOUT_FILENO); /* Duplicate writing end to stdout */
        close(pipefd[0]);
        close(pipefd[1]);
        
        execl(shell, fshel, "-c", arg, NULL);
        exit(EXIT_FAILURE);
        break;

      default : //parent process
        close(pipefd[1]); /* Close writing end of pipe */
        FILE *cmd_output = fdopen(pipefd[0], "r");

        while(fgets(buf, sizeof buf, cmd_output)) /* Read from the pipe */
        {
          int linelen = strlen(buf);
          date_time = realloc(date_time, sizeof(char*) * (linelen -15 + 1));// -15 so we dont include the string last modified 
          MALLOC_CHECK(date_time)
          for(int i = 0; i < linelen;i++)
          {
            date_time[i] = buf[i+15];// plus 15 so we dont include the string (last modified)
          }
        }
        close(pipefd[0]); /* Close read end */
        if(date_time == NULL)
        {
          fprintf(stderr, "bake : Could not attain date information of URL dependency \"%s\"\n",Dstr);
          exit(EXIT_FAILURE);
        }

        wait(&status);
        if(status) /* non zero return = bad */
        {
          fprintf(stderr, "bake : Child process failed in getting date information of URL dependency \"%s\"\n",Dstr);
          exit(EXIT_FAILURE);
        }
        break;
    }
  time_t epochtime = curl_getdate(date_time, NULL);// using curl function to get epoch time
  if(epochtime == -1)
  {
    fprintf(stderr, "bake : Modification date return in unknown format\n");
    exit(EXIT_FAILURE);
  }
  return epochtime;
}
