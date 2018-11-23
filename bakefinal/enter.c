/*****************************************************************************
*    enter.c : Part of CITS2002 project 2018                                 *
* Authors: Danham Wimbridge Glassford : 21987459                             *
*       &  Jay Mummery                : 21984208                             *
* Purpose: Enters action lines into the shell.                               *
* Notes: An action line may have only one modifier (either '-' OR '@') and it*
*       must be the first non-whitespace character in the line.              *
*****************************************************************************/

#include"header.h"

void enter(char *actline, optn argo)
{
  bool ignore = false; /* If true non-zero returns from actions are ignored */
  bool silent = false; /* If true do not print each shell-command-sequence before it is executed */

  char *shell = getenv("SHELL"); /* Returns a pointer with necessary memory allocated */
  char *fshel = basename(shell); /* Gets file name from full path */
  if(!shell || !fshel) /* If either function returned a null pointer use default values*/
  {
    shell = strdup("/bin/bash");
    MALLOC_CHECK(shell)
    fshel = strdup("bash");
    MALLOC_CHECK(fshel)
  }

  actline = trim(actline); /* Get rid of leading/trailing white space */

  switch (actline[0])
  {
    case '-' :
      ignore = true;
      actline++;
      break;

    case '@' :
      silent = true;
      actline++;
      break;

    default :
      /* Do nothing */
      break;
  }

  if(!silent && !argo.sln) /* neither silent nor argo.sln */
  {
    printf("%s\n",actline);
  }

  if(!argo.dne)
  {
    int status;
    int pid = fork();
    switch (pid)
    {
      case -1 :
        fprintf(stderr, "bake : Fatal forking failure on entry of action line : \"%s\"\n",actline);
        exit(EXIT_FAILURE);
        break;

      case 0 :
        execl(shell, fshel, "-c", actline, NULL);
        exit(EXIT_FAILURE);
        break;

      default :
        wait(&status);
        if(!ignore && !argo.ign && status) /* True if return was non zero and neither ignore flags are true */
        {
          fprintf(stderr, "bake : Child process failed on entry of action line : \"%s\"\n",actline);
          exit(EXIT_FAILURE);
        }
        break; 
    }
  }
}
