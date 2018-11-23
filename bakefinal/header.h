#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/wait.h>
#include<libgen.h>
#include<math.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<curl/curl.h>

#if defined(__linux__)
extern char *strdup(const char *str);
extern int getopt(int argc, char * const argv[], const char *optstring);
extern char*optarg;
extern int optind, opterr, optopt;
FILE *fdopen(int fd, const char *mode);
#endif

#define MALLOC_CHECK(x) if(x == NULL){ \
                        fprintf(stderr, "bake : Fatal memory allocation error!\n");\
                        fprintf(stderr, "bake : variable '%s' remains NULL\n", #x);\
                        exit(EXIT_FAILURE); } /* Closing semicolon not necesary after this ''function'' */

/* A structure to keep our command-line arguments together */
typedef struct _o
{
  char *tar; /* Target name */
  char *dir; /* Name of directory to change to */
  char *fil; /* Name of file used in place of Bakefile */
  bool prt; /* If true print the internal representation of Bakefile */
  bool dne; /* If true do not execute action lines */
  bool ign; /* If true ignore all exit statuses */
  bool sln; /* If true do not print any action lines before execution */
} optn;

/* A stuructureto keep our targets & dependencies */
typedef struct _t
{
  char *tar; /* Target name */
  char **deps; /* Array of dependencies */
  int plc; /* The line number in the spec file */
  struct _t **ptrs; /* Array of pointers to dependencies */
} trgs;

/* Global functions */
extern void remove_comments(char**);
extern void replace(char**);
extern bool is_def(char*);
extern bool is_tar(char*);
extern bool is_act(char*);
extern void enter(char*, optn);
extern char *trim(char*);
extern char **find_Bakefile(char*);
extern optn read_args(int, char**);
extern bool needs_upd(char*, char**);
extern trgs *read_trgs(char**, char*);
extern int tar_match(char*, trgs*);
extern bool exists(char*);
extern bool dep_match(char*, char**);
extern void build_target(trgs, char**, optn, char*);
extern void circular(trgs*);
extern bool is_URL(char*);
extern time_t url_date(char*);

/* Global variables */
extern int number_of_lines;
