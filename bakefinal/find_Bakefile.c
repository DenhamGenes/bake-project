/*****************************************************************************
*    find_Bakefile.c : Part of CITS2002 project 2018                         *
* Authors: Danham Wimbridge Glassford : 21987459                             *
*       &  Jay Mummery                : 21984208                             *
* Purpose: Finds the designated bakefile in the current directory, opens it, *
*          and copies its contents line by line into an array of strings.    *
* Restrct: No line of the bakefile may exceed BUFSIZ.                        *
* Notes:   If the command line target specified is "Bakefile" then if Bakefi-*
*          le does not exist but bakefile does bakefile will be used. Any ot-*
*          her target name will only be looked for in one case. Most macOS f-*
*          ile systems are not case sensitive so this is only a special case *
*          on other unix systems.                                            *
*****************************************************************************/

#include "header.h"

char **find_Bakefile(char *fil)
{
    char buffer[BUFSIZ]; /* BUFSIZ defined in stdio.h */
    char **arrayofstr = NULL; /* Will store the lines of Bakefile */
    int linecounter = 0;  
    FILE   *fp1 = fopen("Bakefile", "r");
    FILE   *fp2 = fopen("bakefile", "r");
    FILE   *fp3 = fopen(fil, "r");
    FILE   *spec = NULL; /* The fp# we read from */

    if(!strcmp(fil,"Bakefile")) /* In case sensitive file systems (linux) entering "Bakefile" falls through to "bakefile" but no other target does */
    {
        if(fp1)
        {
            spec = fp1;
        }
        else if(fp2)
        {
            spec = fp2;
        }
        else
        {
          fprintf(stderr, "bake : No Bakefile Found\n");
          exit(EXIT_FAILURE);
        }
    }
    else
    {
        if(fp3)
        {
            spec = fp3;
        }   
        else
        {
            fprintf(stderr, "bake : Cannot find file \"%s\"\n", fil);  
            exit(EXIT_FAILURE); 
        }
    }

    while(fgets(buffer, sizeof buffer, spec)) 
    {
        int line_length = strlen(buffer) +1;

        /* Resize array then copy string plus allocating space to each line of the bakefile into the arrayofstrings*/
        arrayofstr = realloc(arrayofstr, sizeof(char*) * (linecounter + 1));
        MALLOC_CHECK(arrayofstr)
        arrayofstr[linecounter]= malloc(sizeof(char)*(line_length));
        MALLOC_CHECK(arrayofstr[linecounter])

        strcpy(arrayofstr[linecounter], buffer);
        ++linecounter;
    }

     if(fp1) fclose(fp1); /* if prevents closing null-pointers */
     if(fp2) fclose(fp2);
     if(fp3) fclose(fp3);
     number_of_lines = linecounter; /* Global variable */

     return arrayofstr;// returns an array of strings, with each line of the backfile as a seperate string
}
