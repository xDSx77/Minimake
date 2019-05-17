#include "help/help.h"
#include "execution/execution.h"
#include "makefile/makefile.h"

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

int check_args(int argc, char *argv[], FILE **file, char **name, bool *other)
{
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-h"))
        {
            help();
            return 0;
        }
        if (!strcmp(argv[i], "-f"))
        {
            *other = true;
            if (i + 1 < argc)
            {
                *name = argv[i + 1];
                *file = fopen(*name, "re");
                if (!file)
                {
                    nofile(argv[i + 1]);
                    notarget(argv[i + 1]);
                    return 1;
                }
            }
            else
            {
                notarget(argv[i] + 1);
                help();
                return 1;
            }
        }
    }
    return 0;
}

int open_default(FILE **file, char **name)
{
    *name = "makefile";
    *file = fopen(*name, "re");
    if (!*file)
    {
        *name = "Makefile";
        *file = fopen(*name, "re");
        if (!*file)
        {
            nomakefile();
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    char *file_name = "";
    bool other_file = false;
    if (check_args(argc, argv, &file, &file_name, &other_file))
        return 1;
    if (!other_file)
        if (open_default(&file, &file_name))
            return 1;
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
            if (strcmp(argv[i], makefile->rules[j]->target) != 0
                    && strcmp(argv[i], "-f") != 0
                    && strcmp(argv[i], file_name) != 0
                    && makefile->rules[j + 1]->target == NULL)
                notarget(argv[i]);
        }
    if (argc == 1 || (argc == 3 && other_file))
        execute(makefile, makefile->rules[0]);
    free_all(makefile, lines);
    fclose(file);
    return 0;
}
