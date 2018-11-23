/*****************************************************************************
*    read_trgs.c : Part of CITS2002 project 2018                             *
* Authors: Danham Wimbridge Glassford : 21987459                             *
*       &  Jay Mummery                : 21984208                             *
* Purpose: Reads target lines from the bakefile into our 'trgs' structure (d-*
*          efined in header.h). These sructures contain the name of the targ-*
*          et, an array of names of dependencies, the bakefile line the targ-*
*          et appears in (after remove_replace.c has done it's thing), and an*
*          array of pointers to trgs, which points to the target's dependenc-*
*          ies that are themselves targets.                                  *
* Notes:   Every array-member of the target structure is null-terminated and *
*          this is relied on by the functions that use it. These functions a-*
*          re where it is checked that: bakefile has atleast one target line,*
*          the command line target option does match one of the targets, and *
*          every dependency can be resolved i.e. it is one of the targets, or*
*          a readable file, a URL. If a command line target option is provid-*
*          ed that target structure is put in first position i.e. targets[0] *
*          is always the trgs structure of the build target.                 *
*****************************************************************************/

#include"header.h"

trgs fill_targ(char *targetline)
{
  trgs target;
  int p = 0; /* Position of ':' */
  target.deps = NULL; /* For first realloc */

  for(int n = 0; n <= strlen(targetline); n++)
  {
    if(targetline[n] == ':')
    {
      p = n;
      target.tar = malloc(sizeof(char) * (n+1)); /* Left hand side of ':' including a space for '\0' */
      MALLOC_CHECK(target.tar)

      /* strcpy routines from remove_replace */
      for(int m = 0; m < n; m++)
      {
        target.tar[m] = targetline[m];
      }
      target.tar[n] = '\0';
      target.tar = trim(target.tar);
      break; /* One target per line */
    }
  }

  char *ch = targetline + (p+1);
  int q = 0; /* Word counter */
  while(*ch) /* From ':' to '\0' */
  {
     char word[BUFSIZ];
     char *w = word;

     while(!isspace(*ch))
     {
       *w++ = *ch++; /* Copy across character then increment along */
     }
     *w  = '\0';

     if(w != word) /* only count non-null words */
     {
       target.deps = realloc(target.deps, sizeof(char*) * (q+1));
       MALLOC_CHECK(target.deps)
       target.deps[q] = strdup(word);
       MALLOC_CHECK(target.deps[q])
       q++;
     }

     ch++;
  }
  target.deps = realloc(target.deps, sizeof(char*) * (q+1));
  MALLOC_CHECK(target.deps)
  target.deps[q] = NULL; /* For controlling loops */
  
  return target;
}

trgs *swapl(trgs *targets, char *cl_tar)
{
  if(*cl_tar)
  {
    bool matched = false;
    trgs temp;
    int i = 0;

    while(targets[i].tar)
    {
      if(!strcmp(targets[i].tar, cl_tar))
      {
        matched = true;
        temp = targets[i];
        break;
      }  
      i++;
    }

    if(!matched)
    {
      fprintf(stderr, "bake : \"%s\" does not match any target.\n", cl_tar);
      exit(EXIT_FAILURE);
    }

    for(int j = i; j > 0; j--)
    {
      targets[j] = targets[j-1];
    }
    targets[0] = temp;
  }

  return targets;
}

void fill_ptrs(trgs *target_arr)
{
  for(int i = 0; target_arr[i].tar; i++)
  {
    int m = 0; /* ptrs counter */
    int q; /* Dependency position */
    target_arr[i].ptrs = NULL; /* for first realloc */
    for(int n = 0; target_arr[i].deps[n]; n++)
    {
      q = tar_match(target_arr[i].deps[n], target_arr);
      if(q != -1)
      {
        target_arr[i].ptrs = realloc(target_arr[i].ptrs, sizeof(trgs*) * (m+1));
        MALLOC_CHECK(target_arr[i].ptrs)

        target_arr[i].ptrs[m] = &target_arr[q];
        m++;
      }
      else if(!exists(target_arr[i].deps[n]))
      {
        fprintf(stderr, "bake : Nothing to resolve \"%s\" dependency of \"%s\"\n", target_arr[i].deps[n], target_arr[i].tar);
        exit(EXIT_FAILURE);
      }
    }
    target_arr[i].ptrs = realloc(target_arr[i].ptrs, sizeof(trgs*) * (m+1));
    MALLOC_CHECK(target_arr[i].ptrs)
    target_arr[i].ptrs[m] = NULL;
  }
}

trgs *read_trgs(char **b_spec, char *cl_tar)
{
  trgs *targets = NULL; /* required for first realloc (atleast on my machine) */
  int j = 0; /* target counter */

  for(int i = 0; i < number_of_lines; i++)
  {
    if(is_tar(b_spec[i]))
    {
      targets = realloc(targets, sizeof(trgs) * (j+1));
      MALLOC_CHECK(targets);
      targets[j] = fill_targ(b_spec[i]);
      targets[j].plc = i;
      j++;
    }
  }
  if(targets)
  {
      targets = realloc(targets, sizeof(trgs) * (j+1));
      MALLOC_CHECK(targets);
      targets[j] = (trgs) { NULL, NULL, 0, NULL}; /* A sentinal value for all your controlly needs */
  }
  else
  {
    fprintf(stderr, "bake : Specification file contains no target lines!\n");
    exit(EXIT_FAILURE);
  }

  targets = swapl(targets, cl_tar); /* Put build-target in position 0 */

  fill_ptrs(targets); 
 
  return targets;
}
