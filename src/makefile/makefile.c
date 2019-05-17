#include "makefile.h"
#include "../parse/parse.h"
#define SIZEBUF 128

struct makefile *create_struct(FILE *file, int lines)
{
    struct makefile *makefile = malloc(sizeof(struct makefile));
    makefile->vars = malloc(lines * sizeof(struct var*));
    for (int i = 0; i < lines; i++)
    {
        makefile->vars[i] = malloc(sizeof(struct var));
        makefile->vars[i]->data = malloc(SIZEBUF * sizeof(char *));
    }
    makefile->rules = malloc(lines * sizeof(struct rule*));
    for (int i = 0; i < lines; i++)
    {
        makefile->rules[i] = malloc(sizeof(struct rule));
        makefile->rules[i]->dependencies_c = malloc(SIZEBUF * sizeof(char *));
        makefile->rules[i]->commands = malloc(lines * SIZEBUF * sizeof(char *));
        for (int j = 0; j < lines; j++)
            makefile->rules[i]->commands[j] = malloc(SIZEBUF * sizeof(char *));
    }
    if (parse(makefile, file))
        return NULL;
    return makefile;
}
