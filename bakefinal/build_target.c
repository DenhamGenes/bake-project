/*****************************************************************************
*    build_target.c : Part of CITS2002 project 2018                          *
* Authors: Danham Wimbridge Glassford : 21987459                             *
*       &  Jay Mummery                : 21984208                             *
* Purpose: Calls itself on all the dependencies then checks if it needs to b-*
*          e updated and enters action lines into shell if so.               *
* Notes:   ".ptrs[]" must end in a null-pointer. This function will shit the *
*          bed if given any circular dependencies.                           * 
*****************************************************************************/

#include"header.h"

void build_target(trgs target, char **b_spec, optn argo, char *build_tar)
{
  for(int i = 0; target.ptrs[i]; i++) /* .ptrs is null-terminated array of (trgs*) dependencies */
  {
    build_target(*target.ptrs[i], b_spec, argo, build_tar);
  }

  if(needs_upd(target.tar, target.deps))
  {
    for(int n = target.plc + 1; n < number_of_lines && is_act(b_spec[n]); n++) /* Checks it's not off the end before calling is_act */
    {
      enter(b_spec[n], argo); /* Enter in to shell */
    }
  }
  else if(!strcmp(target.tar, build_tar)) /* Only print if it is the build-target */
  {
    fprintf(stdout, "\"%s\" is up to date.\n", target.tar);
  }
}
