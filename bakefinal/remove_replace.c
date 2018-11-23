/**********************************************************************************
*    remove_replace.c : Part of CITS2002 project 2018                             *
* Authors:      Danham Wimbridge Glassford : 21987459                             *
*       &       Jay Mummery                : 21984208                             *
* Purpose:      Contains 2 functions called from main: remove_comments() and repl-*
*               ace() which together take the raw bakefile and convert into what  *
*               is printed by the '-p' option.                                    *
* Restrictions: The '#' must occur at the beginning of the line for a comment to  *
*               be removed. Every line in the bakefile must be either a comment, a*
*               variable definition, a target line, an action line, or be white s-*
*               pace characters only. Every line MUST finish in "\n\0" or various *
*               lines and variables will be truncated.                            *
* Notes:        This was the first source file written for this project. It does  *
*               some things in a less than chic way e.g. uses a 2D array of strin-*
*               gs where a structure might be more natural, and is over-all much  *
*               more verbose in style than the newer functions e.g. take the cust-*
*               om strcpy routines here vs those in read_trgs(). We decided to le-*
*               ave a fair amount of it as it was originally because: it's not to-*
*               o hard to read, the extra changes would've had no effect on the r-*
*               unning of the program, it accurately shows what I was able to cod-*
*               e six weeks ago, and it was already well tested before we learnt  *
*               about the relevant material. Also I never got around to sorting o-*
*               ut the dec/def things so sorry but declarations and definitions a-*
*               re refering to the same thing (NAME = VALUE). Lastly rand() is no-*
*               t seeded with any varying value and so will produce the same value*
*               every time, while this may not seem ideal it is the best interpre-*
*               tation of what the project spec asked for.                        *
**********************************************************************************/

#include"header.h"

int num_dec; /* Number of declaration lines in the Bakefile, accesible by all functions in this file */

void remove_comments(char **b_spec)
{
  char **temp = malloc(sizeof(char*) * number_of_lines);
  MALLOC_CHECK(temp)
  int i = 0, j = 0; /* i counts though all lines in bakefile, j through non-comment lines */

  while(i < number_of_lines)
  {
    if(b_spec[i][0] != '#') /* # must appear in position 0 */
    {
      temp[j] = malloc(sizeof(char) * (strlen(b_spec[i]) + 1));
      MALLOC_CHECK(temp[j])
      strcpy(temp[j],b_spec[i]);
      b_spec[j] = realloc(b_spec[j], sizeof(char) * (strlen(temp[j]) + 1)); /* resizes each line of buffer */
      MALLOC_CHECK(b_spec[j])
      
      strcpy(b_spec[j], temp[j]);
      j++;
    }
    i++;
  }
  number_of_lines = j; /* new number of bakefile lines */
  free(temp);
}

char *trim(char *str)
{
  char *end = str + strlen(str);;

  /* Moves starting pointer along */
  while(isspace(*str))
  {
    str++;
  }

  /* Move end pointer back */
  while(isspace(*(end-1)))
  {
    end--;
  }

  *end = '\0'; /* Terminates string */
  return str;
}


void store_declarations(char **buffer, char ***declarations) /* Stores declarations in a 2D string-array */
{
  int j = 0; /* Counts over the declaration lines */
  for(int i = 0; i < num_dec; i++)
  {
    declarations[i] = malloc(sizeof(char*) * 2); /* Creates two 'columns' per declaration */
    MALLOC_CHECK(declarations[i]);
  }
  
  for(int i = 0; i < number_of_lines; i++)
  {
    if(is_def(buffer[i]))
    {
      for(int n = 0; n <= strlen(buffer[i]); n++)
      {
        if(buffer[i][n] == '=')
        {
           declarations[j][0] = malloc(sizeof(char) * (n+1)); /* Left hand side of '=' including a space for '\0' */
           MALLOC_CHECK(declarations[j][0])

           declarations[j][1] = malloc(sizeof(char) * (strlen(buffer[i])-n)); /* Right hand side of '=' with one fewer space as '\n' must be removed */
           MALLOC_CHECK(declarations[j][1])

           /* Custom strcpy routines to deal with the '\0' and '\n' situation here */
           for(int m = 0; m < n; m++)
           {
             declarations[j][0][m] = buffer[i][m];
           }
           declarations[j][0][n] = '\0';

           for(int m = n+1; m < (strlen(buffer[i]) - 1); m++)
           {
             declarations[j][1][m-n-1] = buffer[i][m];
           }
           declarations[j][1][strlen(buffer[i])-1] = '\0';

           declarations[j][0] = trim(declarations[j][0]); /* Remove leading/trailing white space */
           declarations[j][1] = trim(declarations[j][1]);

           break; /* One declaration per line */
        }
      }
      j++;
    }
  }
}

void remove_declarations(char **buffer) /* Very similar lay out to remove_comments */ 
{
  char **temp = malloc(sizeof(char*) * number_of_lines);
  MALLOC_CHECK(temp)
  int i = 0, j = 0; /* i counts though all lines in bakefile, j through non-definition lines */

  while(i < number_of_lines)
  {
    if(!is_def(buffer[i]))
    {
      temp[j] = malloc(sizeof(char) * (strlen(buffer[i]) + 1));
      MALLOC_CHECK(temp[j])
      strcpy(temp[j],buffer[i]);

      buffer[j] = realloc(buffer[j], sizeof(char) * (strlen(temp[j]) + 1)); /* resizes each line of buffer */
      MALLOC_CHECK(buffer[j])
      strcpy(buffer[j], temp[j]);
      j++;
    }
    i++;
  }
  number_of_lines = j; /* new number of bakefile lines */
  free(temp);
}

char* sub(char *buffer, char ***declarations, int pos, int rep)
{
  char *bef, *aft; /* Stores the text before and after the variable */
  int len1, len2, lenf; /* Lengths of variable, its replacement, and the final line length */
  len1 = strlen(declarations[rep][0]);
  len2 = strlen(declarations[rep][1]);

  bef = malloc(sizeof(char) * (pos+1));
  MALLOC_CHECK(bef)
  for(int i = 0; i < pos; i++)
  {
    bef[i] = buffer[i];
  }
  bef[pos] = '\0';

  aft = malloc(sizeof(char) * (strlen(buffer) - pos - len1 - 2));
  MALLOC_CHECK(aft)
  for(int i = pos + len1 + 3; i <= strlen(buffer); i++)
  {
    aft[i-pos-len1-3] = buffer[i];
  }

  lenf = strlen(bef) + len2 + strlen(aft);

  buffer = realloc(buffer, lenf + 1);
  MALLOC_CHECK(buffer);
  
  strcpy(buffer, bef);
  strcat(buffer, declarations[rep][1]);
  strcat(buffer, aft);
  MALLOC_CHECK(buffer);

  return buffer;
}

void make_replacements(char **buffer, char ***declarations)
{
  int pos, rep; /* Stores the position of '$' in the line and which replacement needs to be made */
  char *var; /* Stores the variable name to be replaced */
  bool matched; /* Whether or not the variable in the text has been matched to a definition */
  for(int i = 0; i < number_of_lines; i++)
  {
    for(int n = 0; n < strlen(buffer[i]) - 1; n++) /* Stops two short of end of line so n+2 is part of the line */
    {
      if((buffer[i][n] == '$') && (buffer[i][n+1] == '('))
      {
        pos = n;
        int m = 0;
        while(buffer[i][n+m+2] != ')')
        { 
          if(n+m+2 >= strlen(buffer[i]))
          {
            fprintf(stderr, "bake : Closing parenthesis missing in variable definition\n");
            exit(EXIT_FAILURE);
          }
          m++;
        }

        var = malloc(sizeof(char) * (m+1)); /* +1 for the '\0' so we can use string functions */
        MALLOC_CHECK(var)

        for(int k = n + 2; k < n + m + 2; k++)
        {
          var[k-n-2] = buffer[i][k];
        } 
        var[m] = '\0';

        /* Check for special cases, else definition lines, else null-string */
        matched = false;
        if(!strcmp(var, "PID"))
        {
          matched = true;
          char **spec = malloc(sizeof(char*) * 2); /* A 2 element array of strings: var and result */
          MALLOC_CHECK(spec)
          spec[0] = strdup(var);
          MALLOC_CHECK(spec[0])
          spec[1] = malloc(sizeof(char*) * 9); /* Max PID on 64-bit machine = 2^22 = 7 decimal digits, we allow 8 */
          MALLOC_CHECK(spec[1])
          snprintf(spec[1], 8, "%d", getpid()); /* No checking getpid doesn't fail */

          buffer[i] = sub(buffer[i], &spec, pos, 0);
        }
        else if(!strcmp(var, "PPID"))
        {
          matched = true;
          char **spec = malloc(sizeof(char*) * 2); /* A 2 element array of strings: var and result */
          MALLOC_CHECK(spec)
          spec[0] = strdup(var);
          MALLOC_CHECK(spec[0])
          spec[1] = malloc(sizeof(char*) * 9); /* Max PID on 64-bit machine = 2^22 = 7 decimal digits, we allow 8 */
          MALLOC_CHECK(spec[1])
          snprintf(spec[1], 8, "%d", getppid()); /* No checking getppid doesn't fail */

          buffer[i] = sub(buffer[i], &spec, pos, 0);
        }
        else if(!strcmp(var, "PWD"))
        {
          matched = true;
          char **spec = malloc(sizeof(char*) * 2); /* A 2 element array of strings: var and result */
          MALLOC_CHECK(spec)
          spec[0] = strdup(var);
          MALLOC_CHECK(spec[0])
          MALLOC_CHECK((spec[1] = getcwd(0,0))) /* Avoids new error message by printing whole expression on failure. zero-zero arguments allow function to allocate necesary memory for full path name */

          buffer[i] = sub(buffer[i], &spec, pos, 0);
        }
        else if(!strcmp(var, "RAND"))
        {
          matched = true;
          char **spec = malloc(sizeof(char*) * 2); /* A 2 element array of strings: var and result */
          MALLOC_CHECK(spec)
          spec[0] = strdup(var);
          MALLOC_CHECK(spec[0])
          int rand_max = (int) ceil(log10(RAND_MAX)); /* Max number of decimal digits for rand() */
          spec[1] = malloc(sizeof(char*) * (rand_max+1)); 
          MALLOC_CHECK(spec[1])
          snprintf(spec[1], rand_max, "%d", rand()); /* Not seeded */

          buffer[i] = sub(buffer[i], &spec, pos, 0);
        }


        if(!matched)
        {
          for(int j = 0; j < num_dec; j++)
          {           
            if(!strcmp(declarations[j][0], var))
            {
              matched = true;
              rep = j;
            }
          }
          if(matched)
          {
            buffer[i] = sub(buffer[i], declarations, pos, rep); /* Buffer[i] now points to a new string containing the replacement. This needs further checking */ 
          }
          else
          {
            char *p; /* Contains environment variable if it exists */
            char **empt = malloc(sizeof(char*) * 2); /* A 2 element array of strings: var and either NULL or environ */
            MALLOC_CHECK(empt)
            empt[0] = strdup(var);
            MALLOC_CHECK(empt[0])
            empt[1] = calloc(1, sizeof(char));
            MALLOC_CHECK(empt[1])
            if( (p = getenv(var)) )
            {
              empt[1] = p;
            }

            buffer[i] = sub(buffer[i], &empt, pos, 0);
          }
        }
        n--; /* Moves counter back incase current char is now '$' again */
        /* No break statement as there can be more than one replacement per line */
      }
    }
  }
}

void remove_blanks(char **buffer)
{
  for(int i = 0; i < number_of_lines; i++)
  {
    char *temp = strdup(buffer[i]); /* avoid overwriting valid lines */
    if(trim(temp)[0] == '\0')
    {
      for(int j = i; (j+1) < number_of_lines; j++)
      {
        buffer[j] = strdup(buffer[j+1]);
      }
      number_of_lines--;
      i--;
    }
    else if(temp[strlen(temp) - 1] == '\\') /* Combine lines where last non-white-space character is '\' */
    {
      char temp_2[BUFSIZ];
      temp[strlen(temp) - 1] = '\0';
      char *w = temp_2, *v = temp, *y = buffer[i+1];
     
      while(*v)
      {
        *w++ = *v++;
      }
      while(*y)
      {
        *w++ = *y++;
      }
      *w = '\0';
      buffer[i] = strdup(temp_2);

      for(int j = (i+1); (j+1) < number_of_lines; j++)
      {
        buffer[j] = strdup(buffer[j+1]);
      }
      number_of_lines--;
      i--;
    } 
  }
}

void replace(char **b_spec)
{
  num_dec =0;
  char ***declarations; /* Pointer to a 2 by num_dec array of strings */

  for(int i = 0; i < number_of_lines; i++)
  {
    if(is_def(b_spec[i]))
    {
      num_dec++; /* I realised here I switched between calling then "definitions" and "declarations" I will change everything to definition later */
    }
  }

  declarations = malloc(sizeof(char**) * num_dec);
  MALLOC_CHECK(declarations) 

  store_declarations(b_spec, declarations);

  remove_declarations(b_spec);

  make_replacements(b_spec, declarations);

  remove_blanks(b_spec); /* Also combines lines ending in "\\\n"*/

  for(int i = 0; i < number_of_lines; i++)
  {
    if(!is_tar(b_spec[i]) && !is_act(b_spec[i]))
    {
      fprintf(stderr, "bake : Unrecognised syntax in line : \"%s\"\n", trim(b_spec[i]));
      exit(EXIT_FAILURE);
    }
  }
}
