#include "makefile.h"
#include "../parse/parse.h"
#define SIZEBUF 128

struct makefile *create_struct(FILE *file, int lines)
{
    struct makefile *makefile = calloc(1, sizeof(struct makefile));
    makefile->vars = calloc(lines, sizeof(struct var*));
    for (int i = 0; i < lines; i++)
    {
        makefile->vars[i] = calloc(1, sizeof(struct var));
        makefile->vars[i]->data = calloc(SIZEBUF, sizeof(char *));
        makefile->vars[i]->nb_data = 0;
    }
    makefile->rules = calloc(lines, sizeof(struct rule*));
    for (int i = 0; i < lines; i++)
    {
        makefile->rules[i] = calloc(1, sizeof(struct rule));
        makefile->rules[i]->dependencies_c = calloc(SIZEBUF, sizeof(char *));
        makefile->rules[i]->commands = calloc(lines * SIZEBUF, sizeof(char *));
        for (int j = 0; j < lines; j++)
            makefile->rules[i]->commands[j] = calloc(SIZEBUF, sizeof(char *));
        makefile->rules[i]->lines = 0;
        makefile->rules[i]->nb_dependencies = 0;
        makefile->rules[i]->nb_commands = calloc(SIZEBUF, sizeof(int));
    }
    makefile->nb_vars = 0;
    makefile->nb_rules = 0;
    if (parse(makefile, file))
        return NULL;
    return makefile;
}
