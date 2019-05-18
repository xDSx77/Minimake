#include "help/help.h"
#include "execution/execution.h"
#include "makefile/makefile.h"

void free_all(struct makefile *makefile, int lines)
{
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < makefile->vars[i]->nb_data; j++)
            free(makefile->vars[i]->data[j]);
        if (i < makefile->nb_vars)
            free(makefile->vars[i]->name);
        free(makefile->vars[i]->data);
        free(makefile->vars[i]);
        struct rule *cur_rule = makefile->rules[i];
        for (int j = 0; j < cur_rule->lines; j++)
            for (int k = 0; k < cur_rule->nb_commands[j]; k++)
                free(cur_rule->commands[j][k]);
        for (int j = 0; j < lines; j++)
            free(cur_rule->commands[j]);
        free(cur_rule->commands);
        free(cur_rule->nb_commands);
        if (i < makefile->nb_rules)
            free(makefile->rules[i]->target);
        if (makefile->rules[i]->dependencies_c)
            for (int j = 0; j < makefile->rules[i]->nb_dependencies; j++)
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
                    exit(1);
                }
            }
            else
            {
                notarget(argv[i] + 1);
                help();
                exit(1);
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
            exit(1);
        }
    }
    return 0;
}

void execute_args(int argc, char *argv[], struct makefile *makefile, int lines,
        FILE *file, char *file_name)
{
    for (int i = 1; i < argc; i++)
    {
        for (int j = 0; j < makefile->nb_rules; j++)
        {
            if (makefile->rules[j]->target
                    && strcmp(argv[i], makefile->rules[j]->target) == 0)
            {
                if (execute(makefile, makefile->rules[j]) == 2)
                {
                    free_all(makefile, lines);
                    fclose(file);
                    exit(2);
                }
                break;
            }
            if (strcmp(argv[i], makefile->rules[j]->target) != 0
                    && strcmp(argv[i], "-f") != 0
                    && strcmp(argv[i], file_name) != 0
                    && makefile->rules[j + 1]->target == NULL)
                notarget(argv[i]);
        }
    }
}

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    char *file_name = "";
    bool other_file = false;
    check_args(argc, argv, &file, &file_name, &other_file);
    if (!other_file)
        open_default(&file, &file_name);
    char line[5000];
    int lines = 0;
    while (fgets(line, sizeof(line), file) != NULL)
        lines++;
    rewind(file);
    struct makefile *makefile;
    if (!(makefile = create_struct(file, lines)))
    {
        fclose(file);
        return 1;
    }
    execute_args(argc, argv, makefile, lines, file, file_name);
    if (argc == 1 || (argc == 3 && other_file))
        if (execute(makefile, makefile->rules[0]) == 2)
        {
            free_all(makefile, lines);
            fclose(file);
            return 2;
        }
    free_all(makefile, lines);
    fclose(file);
    return 0;
}
