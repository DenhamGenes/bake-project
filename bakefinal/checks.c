/*****************************************************************************
*    checks.c : Part of CITS2002 project 2018                                *
* Authors: Denham Wimbridge Glassford : 21987459                             *
*       &  Jay Mummery                : 21984208                             *
* Purpose: Contains some of the small check functions called at various plac-*
*          es in our project.                                                *
* Notes:   Restrictions on what it is to be each of the line types are fairly*
*          broad and hence subtly invalid lines will not fail but may not be-*
*          have as expected.
           is_URL only looks for lower case of http,file and https and does
           not recognise the types listed above as a valid URL if there are
           capitals.
 *
*****************************************************************************/

#include"header.h"

/* A line is a target-line if and only if it does not begin with '\t' and it coantains a ':' but not after a '=' */
bool is_tar(char *line)
{
  if(is_act(line))
  {
    return false;
  }
  for(int i = 0; i <= strlen(line); i++)
  {
    if(line[i] == '=')
    {
      return false;
    }
    else if(line[i] == ':')
    {
      return true;
    }
  }
  return false;
}

/* A line is a definition-line if and only if it does not begin with '\t' and it coantains a '=' but not after a ':' */
bool is_def(char *line)
{
  if(is_act(line))
  {
    return false;
  }
  for(int i = 0; i <= strlen(line); i++)
  {
    if(line[i] == ':')
    {
      return false;
    }
    else if(line[i] == '=')
    {
      return true;
    }
  }
  return false;
}

/* A line is an action-line if and only if it begins with '\t' */
bool is_act(char *line)
{
  if(line[0] == '\t')
  {
    return true;
  }
  else
  {
    return false;
  }
}

int tar_match(char *targ, trgs *target_arr)
{
  for(int i = 0; target_arr[i].tar; i++)
  {
    if(!strcmp(targ, target_arr[i].tar))
    {
      return i;
    }
  }
  return -1;// fails returns -1 
}

bool dep_match(char* tar, char **deps)// matches dependancies
{
  for(int i = 0; deps[i]; i++)
  {
    if(!strcmp(tar, deps[i]))
    {
      return true;
    }
  }
  return false;
}

bool exists(char *fil)
{
  struct stat a;
  int dne = stat(fil, &a);
  if(!dne)
  {
    return true;
  }
  else if(is_URL(fil))
  {
    return true;
  }
  return false;
}

char **circular_aux(trgs target, char **list)
{
  /* Update list to include all targets 'visited' so far */
  int i = 0;
  while(list[i])
  {
    i++;
  }
  list = realloc(list, sizeof(char*) * (i+2));
  MALLOC_CHECK(list)

  list[i] = strdup(target.tar);
  MALLOC_CHECK(list[i])
  list[i+1] = NULL;

  /* If dependency is matches any targets dependent on it -> CIRCULAR! */
  for(int j = 0; target.ptrs[j]; j++)
  {
    if(dep_match(target.ptrs[j]->tar, list)) 
    {
      fprintf(stderr, "bake : Circular dependency detected!\n\"%s\" -> \"%s\"\n", target.ptrs[j]->tar, target.ptrs[j]->tar);
      exit(EXIT_FAILURE);
    }
    else
    {
      list = circular_aux(*target.ptrs[j], list);
    }
  }
  return list; /* list is returned as realloc() may move pointer */
}

void circular(trgs *target_arr)
{
  for(int i = 0; target_arr[i].tar; i++)
  {
    /* list is cleared before checking each new build-target */
    char **list = malloc(sizeof(char*) * 1);
    MALLOC_CHECK(list)
    list[0] = NULL;

    circular_aux(target_arr[i], list); /* Recursively checks dependencies against list of targets that depend on it */
  }
}

bool is_URL( char *Dstr)// tells if the  string is a URL or not
{   
    bool is_URL = false;
    char strfil[] = "file";
    char strhttp[] = "http";
    
    int depend_len = strlen(Dstr);
    char str1[depend_len];
    char substring[depend_len];
    int l = 5, c = 0;
    strcpy(str1,Dstr);
   
   while (c < l) //substring checking for the valid URL structure of http or file
   {  
      substring[c] = str1[c];
      c++;
   }
   substring[c] = '\0';
  
  if(strncmp(substring,strfil,strlen(strfil)) == 0 || strncmp(substring, strhttp,strlen(strhttp)) == 0)
  {
    
    for(int i = 0; i < depend_len; ++i)
    {   
       if(str1[i] == ':' && str1[i+1] == '/' && str1[i+2] == '/')// checking for valid characters of a URL and that they are in order
       {   
         is_URL = true;
       }
    }
  }
  else if(Dstr == NULL)// checks if the string passed to the function contained anything
  { 
    return is_URL;
  }
  return is_URL;// returns true if it is a URL, returns false if its is not a URL
}
