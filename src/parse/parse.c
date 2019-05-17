#include "parse.h"

int parse(struct makefile *makefile, FILE *file)
{
    char *line = "";
    size_t len = 0;
    bool is_a_rule = false;
    int cur_rule_idx = 0;
    while (getline(&line, &len, file) != -1)
    {
        if (is_a_rule && line[0] != '\t')
        {
            is_a_rule = false;
            continue;
        }
        if (is_a_rule && line[0] == '\t')
        {
            char **commands = split(line);
            makefile->rules[cur_rule_idx]->nb_commands = 0;
            for (int i = 0; commands[i]; i++)
            {
                makefile->rules[cur_rule_idx]->commands[i] =
                    calloc(strlen(commands[i]) + 1, 1);
                strncpy(makefile->rules[cur_rule_idx]->commands[i], commands[i],
                        strlen(commands[i]));
                makefile->rules[cur_rule_idx]->nb_commands++;
            }
            free(commands);
            continue;
        }
        for (size_t i = 0; i < strlen(line); i++)
        {
            if (line[i] == ' ' || line[i] == '\n')
                continue;
            if (line[i] == '#')
                break;
            if (line[i] == '=')
            {
                if (add_var(makefile, line))
                    return 1;
            }
            else if (line[i] == ':')
            {
                if (add_rule(makefile, line, &cur_rule_idx))
                    return 1;
                is_a_rule = true;
            }
        }
    }
    free(line);
    return 0;
}

int add_var(struct makefile *makefile, char *line)
{
    char *name = strtok(line, "= ");
    if (!name)
        return 1;
    char *data_l = strtok(NULL, "\n#");
    if (!data_l)
        return 1;
    char **data = split(data_l);
    if (!data)
        return 1;
    int i = 0;
    while (makefile->vars[i]->name != NULL)
        i++;
    if (i >= makefile->nb_vars)
        makefile->nb_vars = i + 1;
    makefile->vars[i]->name = calloc(strlen(name) + 1, 1);
    strncpy(makefile->vars[i]->name, name, strlen(name));
    for (int j = 0; data[j] != NULL; j++)
    {
        makefile->vars[i]->data[j] = calloc(strlen(data[j]) + 1, 1);
        strncpy(makefile->vars[i]->data[j], data[j], strlen(data[j]));
        makefile->vars[i]->nb_data++;
    }
    free(data);
    return 0;
}

int add_rule(struct makefile *makefile, char *line, int *rule_idx)
{
    char *target = strtok(line, ": ");
    if (!target)
        return 1;
    char *dependencies_l = strtok(NULL, "\n#");
    char **dependencies = split(dependencies_l);
    int i = 0;
    while (i < makefile->nb_rules)
        i++;
    *rule_idx = i;
    if (i >= makefile->nb_rules)
        makefile->nb_rules = i + 1;
    makefile->rules[i]->target = calloc(strlen(target) + 1, 1);
    strncpy(makefile->rules[i]->target, target, strlen(target));
    if (dependencies != NULL)
    {
        for (int j = 0; dependencies[j] != NULL; j++)
        {
            makefile->rules[i]->dependencies_c[j] =
                calloc(strlen(dependencies[j]) + 1, 1);
            strncpy(makefile->rules[i]->dependencies_c[j], dependencies[j],
                    strlen(dependencies[j]));
            makefile->rules[i]->nb_dependencies++;
        }
        free(dependencies);
    }
    return 0;
}

char **split(char *data_l)
{
    if (data_l == NULL)
        return NULL;
    char **data = malloc(strlen(data_l) * sizeof(char *));
    int i = 0;
    char *token = strtok(data_l, " \t");
    while (token)
    {
        data[i] = token;
        token = strtok(NULL, " \t\n");
        i++;
    }
    data[i] = NULL;
    return data;
}
