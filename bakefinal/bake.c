/*****************************************************************************
*    bake.c : The 'main' of CITS2002 project 2018                            *
* Authors: Danham Wimbridge Glassford : 21987459                             *
*       &  Jay Mummery                : 21984208                             *
* Notes: All errors or messages begin with "bake :", this was chosen over us-*
*        ing argv[0] so as to maintain our brand (also so we didn't have to  *
*        pass argv everywhere). The program has only 2 exit statuses: EXIT_S-*
*        UCCESS and EXIT_FAILURE which encompasses all errors. Both circular-*
*        () and build_target() operate recursively as it was determined that *
*        speed was of little to no concern for this project and other methods*
*        attempted introduced many more problems for only a few milliseconds *
*        gained.                                                             *
*****************************************************************************/

#include"header.h"

int number_of_lines;

int main(int argc, char *argv[])
{
  optn argo = read_args(argc, argv); /* Gets command line arguements in to struct */

  int unchanged = chdir(argo.dir); /* Default is chdir(".") */
  if(unchanged)
  {
    fprintf(stderr, "bake : Unable to navigate to directory : \"%s\"\n", argo.dir);
    exit(EXIT_FAILURE);
  }

  char **b_spec; /* Contains line oriented spec file */
  b_spec = find_Bakefile(argo.fil);
  MALLOC_CHECK(b_spec);

  remove_comments(b_spec);

  replace(b_spec); /* Makes variable replacements, removes blank lines, checks what's left */

  if(argo.prt)
  {
    for(int i = 0; i < number_of_lines; i++)
    {
      printf("%s", b_spec[i]);
    }
    exit(EXIT_SUCCESS);
  }

  trgs *target_arr = read_trgs(b_spec, argo.tar); /* Builds an array of target structures */

  circular(target_arr); /* Checks for circular dependencies */

  build_target(target_arr[0], b_spec, argo, target_arr[0].tar); /* Recursively builds dependencies then target */

  return EXIT_SUCCESS;
}
