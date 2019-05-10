#include "makefile.h"
#include "../parse/parse.h"

struct makefile *create_struct(FILE *file)
{
  int lines = 0;
  char line[INT_MAX];
  while(fgets(line, sizeof(line), file) != NULL){
    lines++;
  }
  struct makefile *makefile = malloc(sizeof(struct makefile));
  makefile->vars = malloc(lines * sizeof(struct var*));
  makefile->rules = malloc(lines * sizeof(struct rule*));
  parse(makefile, file);
  return makefile;
}
