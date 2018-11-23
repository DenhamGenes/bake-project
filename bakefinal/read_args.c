/*****************************************************************************
*    read_args.c : Part of CITS2002 project 2018                             *
* Authors: Danham Wimbridge Glassford : 21987459                             *
*          & Jay Mummery              : 21984208                             *
* Purpose: Reads command line arguments into an 'optn' structure (defined in *
*          header.h) and provides their default values.                      *
* Notes:   It was decided that providing an -option would simply set the val-*
*          ue rather than toggling it back and forth as this is what make do-*
*          es on my machine. We also decided -p and -n were not compatible w-*
*          ith each-other or with -i or -s and hence these combinations throw*
*          out an error. This was because no order of operations was defined *
*          in the project spec and there is no one 'obvious' interpretation  *
*          of what should happen.                                            *
*****************************************************************************/

#include"header.h"

#define OPTLIST "pnisC:f:" /* .tar treated seperately */

/* True if str starts with '-' and ends with  'f' of 'C' */
bool is_FC(char *str)
{
  int end = 0;
  while(str[end+1])
  {
    end++;
  }

  if(str[0] == '-' && (str[end] == 'f' || str[end] == 'C' ) )
  {
    return true;
  }
  return false;
}

optn read_args(int argc, char *argv[])
{
  optn argo; /* argo for 'command-line argument options' */
  int opt; /* Char value of last option read */

  /* Default Values */
  argo.tar = calloc(1, sizeof(char)); /* Single '\0' */
  MALLOC_CHECK(argo.tar)// tar = target 

  argo.dir = malloc(sizeof(char)* 2);
  MALLOC_CHECK(argo.dir)
  strcpy(argo.dir, ".");

  argo.fil = malloc(sizeof(char) * 9);
  MALLOC_CHECK(argo.fil)
  strcpy(argo.fil, "Bakefile");

  argo.prt = false;
  argo.dne = false;//dne does not exist 
  argo.ign = false;
  argo.sln = false;

  /* Edit argo based on command line arguments */
  while((opt = getopt(argc, argv, OPTLIST)) != -1)
  {
    switch (opt)
    {
      case 'p' :
        argo.prt = true;
        break;

      case 'n' :
        argo.dne = true;
        break;

      case 'i' :
        argo.ign = true;
        break;

      case 's' :
        argo.sln = true;
        break;

      case 'C' :
        argo.dir = strdup(optarg);
        MALLOC_CHECK(argo.dir)
        break;

      case 'f' :
        argo.fil = strdup(optarg);
        MALLOC_CHECK(argo.fil);
        break;

      default :
        /* do nothing */
        break;
    }
  }

  /* Check for incompatible options
  -p is incompatible with -n, -i, & -s
  -n is incompatible with -i & -s */
  if( (argo.prt && (argo.dne || argo.ign || argo.sln)) || (argo.dne && (argo.ign || argo.sln)) )
  {
    fprintf(stderr, "bake : Incompatible command line options!\n");
    fprintf(stderr, "bake : Options -p & -n cannot perform with each other or with either of -i or -s\n");
    exit(EXIT_FAILURE);
  }

  /* Edit argo.tar */
  /* An argument is a target if: it is the last argument provided, it does not begin with '-', and is not preceded by "-C" or "-f" */
  if( argc > 1               &&
      argv[argc-1][0] != '-' && 
      !is_FC(argv[argc-2])    )
  {
    argo.tar = strdup(argv[argc-1]);
    MALLOC_CHECK(argo.tar)
  }

  return argo;
}
