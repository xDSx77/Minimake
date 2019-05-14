#include "makefile.h"
#include "../parse/parse.h"

struct makefile *create_struct(FILE *file)
{
  struct makefile *makefile = malloc(sizeof(struct makefile));
  makefile->vars = malloc(5000 * sizeof(struct var*));
  for (int i = 0; i < 5000; i++)
  {
    makefile->vars[i] = malloc(sizeof(struct var));
    makefile->vars[i]->data = malloc(5000 * sizeof(char *));
  }
  makefile->rules = malloc(5000 * sizeof(struct rule*));
  for (int i = 0; i < 5000; i++)
  {
    makefile->rules[i] = malloc(sizeof(struct rule));
    makefile->rules[i]->dependencies_c = malloc(5000 * sizeof(char *));
    makefile->rules[i]->dependencies = malloc(5000 * sizeof(struct rule*));
    for (int j = 0; j < 5000; j++)
      makefile->rules[i]->dependencies[j] = malloc(sizeof(struct rule));
    makefile->rules[i]->commands = malloc(5000 * sizeof(char *));
  }
  if (parse(makefile, file))
    return NULL;
  return makefile;
}
