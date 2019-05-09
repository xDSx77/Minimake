#include "../parse/parse.h"

struct makefile *create_struct(FILE *file)
{
  struct makefile *makefile = malloc(sizeof(struct makefile));
  makefile->vars = malloc(sizeof(struct var));
  makefile->rules = malloc(sizeof(struct rule));
  parse(makefile, file);
  return makefile;
}
