#include <stdio.h>
#include <string.h>
#include "help/help.h"
#include "parse/parse.h"

int main(int argc, char *argv[])
{
  FILE *file;
  for (int i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "-h"))
    {
      help();
      return 0;
    }
    if (!strcmp(argv[i], "-f"))
    {
      if (i+1 < argc)
      {
        file = fopen(argv[i+1], "r");
        if (!file)
        {
          fprintf(stderr, "minimake: %s: No such file or directory\n",
            argv[i+1]);
          return 1;
        }
      }
      else
      {
        printf("minimake: option requires an argument -- '%s'\n", argv[i]);
        help();
        return 1;
      }
    }
  }
  file = fopen("makefile", "r");
  if (!file)
  {
    file = fopen("Makefile", "r");
    if (!file)
    {
      fprintf(stderr, "minimake: *** No targets specified and no makefile \
        found. Stop.\n");
      return 1;
    }
  }
  struct makefile *makefile;
  if (!(makefile = create_struct(file)))
    return 1;
  return 0;
}
