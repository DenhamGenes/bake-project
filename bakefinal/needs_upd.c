/*****************************************************************************
*    needs_upd.c : Part of CITS2002 project 2018                             *
* Authors: Denham Wimbridge Glassford : 21987459                             *
*       &  Jay Mummery                : 21984208                             *
* Purpose: Gets modification dates of targets and dependencies and determines*
*          if the target needs an update or not. checkdate function makes a  *
*          a structure of type stat to store the modification date of the    *
           file in question. This date is returned in the form of a unix     *
           timestamp in seconds from the epoch.
* Notes:                                                                     *
*****************************************************************************/

#include"header.h"

time_t check_date(char *Dstr)
{
    struct stat attributes;
    if(stat(Dstr, &attributes) == -1)// creates a struct of dependency file
    {
      fprintf(stderr, "bake : Could not access date information of \"%s\"\n", Dstr);
      exit(EXIT_FAILURE);
    }
    return attributes.st_mtime; //returns the time in seconds from the epoch date
}

bool needs_upd(char *tar, char **deps)// takes a target and array of dependencies for that specific target
{
    time_t dpen_date = 0;
    time_t tar_date = 0;
    
    if(!exists(tar))// if the target does not exit then compile target line
    {
      return true;
    }
    tar_date = check_date(tar);
    int i = 0;

  while(deps[i])
  {
      if(is_URL(deps[i]))// if denpendancy  is a target then pass to url_date function
      {
        dpen_date = url_date(deps[i]);
      }
      else
      {
        dpen_date = check_date(deps[i]);
      }
    

      if(tar_date < dpen_date)// if target date is older than dependancy date, update and compile target
      {
        return true;
      }
      i++;
  }
  
  if(!deps[0])
  {
    return true; /* No dependancies -> always update */
  } 
  return false; // returns true if the targetline needs to be updated or returns false if the target line is newer than all its dependancies
}
