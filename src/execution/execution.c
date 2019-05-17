#include "execution.h"
#include "../parse/parse.h"
#include "../help/help.h"

char *extract(char *command)
{
    char *extract = malloc(sizeof(char *));
    for (size_t i = 0; i < strlen(command) - 3; i++)
        extract[i] = command[i + 2];
    return extract;
}

void reduce(char **commands, struct rule *rule)
{
    for (int i = 0; i < rule->nb_commands; i++)
    {
        commands[i] = calloc(strlen(rule->commands[i]) + 1, 1);
        int len = strlen(rule->commands[i]);
        if (strlen(rule->commands[i]) > 3 && rule->commands[i][0] == '$'
                && (rule->commands[i][1] == '(' || rule->commands[i][1] == '{')
                && (rule->commands[i][len - 1] == ')'
                    || rule->commands[i][len - 1] == '}'))
        {
            char *extracted = extract(rule->commands[i]);
            strncpy(commands[i], extracted, strlen(extracted));
            free(extracted);
        }
        else
            strncpy(commands[i], rule->commands[i], strlen(rule->commands[i]));
    }
}

void replace(char **commands, struct makefile *makefile)
{
    for (int i = 0; commands[i] != NULL; i++)
    {
        for (int j = 0; j < makefile->nb_vars; j++)
        {
            if (strcmp(commands[i], makefile->vars[j]->name) == 0)
            {
                strcpy(commands[i], "");
                for (int k = 0; makefile->vars[j]->data[k] != NULL; k++)
                {
                    strncat(commands[i], makefile->vars[j]->data[k],
                            strlen(makefile->vars[j]->data[k]));
                    if (makefile->vars[j]->data[k + 1] != NULL)
                        strcat(commands[i], " ");
                }
                break;
            }
        }
    }
}

char **separate(char **commands)
{
    char **commands_exe = calloc(128, sizeof(char *));
    if (!commands_exe)
        return NULL;
    int idx_command = 0;
    bool multiple_strings;
    for (int i = 0; commands[i] != NULL; i++)
    {
        multiple_strings = false;
        for (size_t j = 0; j < strlen(commands[i]); j++)
            if (commands[i][j] == ' ')
            {
                multiple_strings = true;
                break;
            }
        if (multiple_strings)
        {
            char **separate = split(commands[i]);
            for (int j = 0; separate[j] != NULL; j++)
            {
                size_t len = strlen(separate[j]);
                commands_exe[idx_command] = malloc(len);
                strncpy(commands_exe[idx_command], separate[j], len);
                idx_command++;
            }
        }
        else
        {
            size_t len = strlen(commands[i]);
            commands_exe[idx_command] = malloc(len);
            strncpy(commands_exe[idx_command], commands[i], len);
            idx_command++;
        }
    }
    return commands_exe;
}

int execute(struct makefile *makefile, struct rule *rule)
{
    int status;
    int exec_status = 0;
    pid_t child_pid = fork();
    if (child_pid == -1)
    {
        perror("fork");
        exit(1);
    }
    if (child_pid == 0)
    {
        if (rule->dependencies_c && rule->dependencies_c[0])
        {
            for (int i = 0; i < makefile->nb_rules; i++)
            {
                if (strcmp(rule->dependencies_c[0],
                            makefile->rules[i]->target) == 0)
                {
                    exec_status = execute(makefile, makefile->rules[i]);
                    break;
                }
                if (makefile->rules[i + 1]->target == NULL)
                {
                    notarget_dependency(rule->dependencies_c[0], rule->target);
                    return 2;
                }
            }
        }
        if (!exec_status)
        {
            char **commands = calloc(1000, sizeof(char *));
            reduce(commands, rule);
            replace(commands, makefile);
            //char **commands_exe = separate(commands);
            if (commands)
            {
                for (int i = 0; commands[i] != NULL; i++)
                {
                    printf("%s", commands[i]);
                    if (commands[i + 1] != NULL)
                        printf(" ");
                }
                printf("\n");
                execvp(commands[0], commands);
                for (int i = 0; commands[i] != NULL; i++)
                    free(commands[i]);
                free(commands);
                return 0;
            }
        }
        return 2;
    }
    waitpid(child_pid, &status, WUNTRACED);
    return 2;
}
