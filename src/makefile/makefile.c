#include "makefile.h"
#include "../parse/parse.h"

struct makefile *create_struct(FILE *file, int lines)
{
  struct makefile *makefile = malloc(sizeof(struct makefile));
  makefile->vars = malloc(lines * sizeof(struct var*));
  for (int i = 0; i < lines; i++)
  {
    makefile->vars[i] = malloc(sizeof(struct var));
    makefile->vars[i]->data = malloc(1000 * sizeof(char *));
  }
  makefile->rules = malloc(lines * sizeof(struct rule*));
  for (int i = 0; i < lines; i++)
  {
    makefile->rules[i] = malloc(sizeof(struct rule));
    makefile->rules[i]->dependencies_c = malloc(1000 * sizeof(char *));
    makefile->rules[i]->dependencies = malloc(lines * sizeof(struct rule*));
    for (int j = 0; j < lines; j++)
      makefile->rules[i]->dependencies[j] = malloc(sizeof(struct rule));
    makefile->rules[i]->commands = malloc(1000 * sizeof(char *));
  }
  if (parse(makefile, file))
    return NULL;
  return makefile;
}
