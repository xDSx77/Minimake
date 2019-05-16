#include <stdio.h>
#include <string.h>
#include "help/help.h"
#include "makefile/makefile.h"
#include "execution/execution.h"

void free_all(struct makefile *makefile, int lines)
{
  for (int i = 0; i < lines; i++)
  {
    for (int j = 0; makefile->vars[i]->data[j] != NULL; j++)
      free(makefile->vars[i]->data[j]);
    free(makefile->vars[i]->data);
    free(makefile->vars[i]);
    for (int j = 0; makefile->rules[i]->commands[j] != NULL; j++)
      free(makefile->rules[i]->commands[j]);
    free(makefile->rules[i]->commands);
    if (makefile->rules[i]->dependencies_c)
      for (int j = 0; makefile->rules[i]->dependencies_c[j] != NULL; j++)
        free(makefile->rules[i]->dependencies_c[j]);
    free(makefile->rules[i]->dependencies_c);
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
  char *file_name;
  bool other_file = false;
  for (int i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "-h"))
    {
      help();
      return 0;
    }
    if (!strcmp(argv[i], "-f"))
    {
      other_file = true;
      if (i+1 < argc)
      {
        file_name = argv[i+1];
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
  if (!other_file)
  {
    file_name = "makefile";
    file = fopen("makefile", "r");
    if (!file)
    {
      file_name = "Makefile";
      file = fopen("Makefile", "r");
      if (!file)
      {
        fprintf(stderr, "minimake: *** No targets specified and no makefile \
          found. Stop.\n");
        return 1;
      }
    }
  }
  char line[5000];
  int lines = 0;
  while (fgets(line, sizeof(line), file) != NULL)
    lines++;
  rewind(file);
  struct makefile *makefile;
  if (!(makefile = create_struct(file, lines)))
    return 1;
  for (int i = 1; i < argc; i++)
    for (int j = 0; makefile->rules[j]->target != NULL; j++)
    {
      if (strncmp(argv[i], makefile->rules[j]->target,
          strlen(makefile->rules[j]->target)) == 0)
      {
        execute(makefile, makefile->rules[j]);
        break;
      }
      else if (strcmp(argv[i], makefile->rules[j]->target) != 0
        && strcmp(argv[i], "-f") != 0
        && strcmp(argv[i], file_name) != 0
        && makefile->rules[j+1]->target == NULL)
        fprintf(stderr, "minimake: *** No rule to make target '%s'. Stop.\n",
          argv[i]);
    }
  if (argc == 1 || (argc == 3 && other_file))
    execute(makefile, makefile->rules[0]);
  free_all(makefile, lines);
  fclose(file);
  return 0;
}
