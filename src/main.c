#include <stdio.h>
#include <string.h>
#include "help/help.h"
#include "makefile/makefile.h"
#include "execution/execution.h"

void free_all(struct makefile *makefile)
{
  for (int i = 0; i < 5000; i++)
  {
    for (int j = 0; makefile->vars[i]->data[j] != NULL; j++)
      free(makefile->vars[i]->data[j]);
    free(makefile->vars[i]->data);
    free(makefile->vars[i]);
    free(makefile->rules[i]->commands);
    if (makefile->rules[i]->dependencies_c)
      for (int j = 0; makefile->rules[i]->dependencies_c[j] != NULL; j++)
        free(makefile->rules[i]->dependencies_c[j]);
    free(makefile->rules[i]->dependencies_c);
    for (int j = 0; j < 5000; j++)
      free(makefile->rules[i]->dependencies[j]);
    free(makefile->rules[i]->dependencies);
    if (makefile->rules[i])
      free(makefile->rules[i]);
  }
  free(makefile->vars);
  free(makefile->rules);
  free(makefile);
}

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
  for (int i = 1; i < argc; i++)
    for (int j = 0; makefile->rules[j]->target != NULL; j++)
    {
      if (strcmp(argv[i], makefile->rules[j]->target) == 0)
        execute(makefile->rules[j]);
      else if (strcmp(argv[i], makefile->rules[j]->target) != 0
        && makefile->rules[j+1]->target == NULL)
        fprintf(stderr, "minimake: *** No rule to make target '%s'. Stop.\n",
          argv[i]);
    }
  execute(makefile->rules[0]);
  free_all(makefile);
  fclose(file);
  return 0;
}
