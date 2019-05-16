#include "execution.h"
#include "../parse/parse.h"

char *extract(char *command)
{
  char *extract = malloc(sizeof(char *));
  for (size_t i = 0; i < strlen(command) - 3; i++)
    extract[i] = command[i+2];
  return extract;
}

void reduce(char **commands, struct rule *rule)
{
  for (int i = 0; rule->commands[i] != NULL; i++)
  {
    commands[i] = malloc(strlen(rule->commands[i]));
    int len = strlen(rule->commands[i]);
    if (strlen(rule->commands[i]) > 3 && rule->commands[i][0] == '$'
        && (rule->commands[i][1] == '(' || rule->commands[i][1] == '{')
        && (rule->commands[i][len-1] == ')' || rule->commands[i][len-1] == '}'))
    {
      char *extracted = extract(rule->commands[i]);
      strncpy(commands[i], extracted, strlen(extracted));
    }
    else
      strncpy(commands[i], rule->commands[i], strlen(rule->commands[i]));
  }
}

void replace(char **commands, struct makefile *makefile)
{
  for (int i = 0; commands[i] != NULL; i++)
  {
    for (int j = 0; makefile->vars[j]->name != NULL; j++)
    {
      if (strcmp(commands[i], makefile->vars[j]->name) == 0)
      {
        strcpy(commands[i], "");
        for (int k = 0; makefile->vars[j]->data[k] != NULL; k++)
        {
          strncat(commands[i], makefile->vars[j]->data[k],
              strlen(makefile->vars[j]->data[k]));
          if (makefile->vars[j]->data[k+1] != NULL)
            strcat(commands[i], " ");
        }
        break;
      }
    }
  }
}

char **separate(char **commands)
{
  char **commands_exe = malloc(128 * sizeof(char *));
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
        commands_exe[idx_command] = malloc(strlen(separate[j]));
        strncpy(commands_exe[idx_command], separate[j], strlen(separate[j]));
        idx_command++;
      }
    }
    else
    {
      commands_exe[idx_command] = malloc(strlen(commands[i]));
      strncpy(commands_exe[idx_command], commands[i], strlen(commands[i]));
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
      for (int i = 0; makefile->rules[i]->target != NULL; i++)
      {
        if (strcmp(rule->dependencies_c[0], makefile->rules[i]->target) == 0)
        {
          exec_status = execute(makefile, makefile->rules[i]);
          break;
        }
        if (makefile->rules[i+1]->target == NULL)
        {
          fprintf(stderr, "minimake: *** No rule to make target '%s', needed by '%s'. Stop.\n",
              rule->dependencies_c[0], rule->target);
          return 2;
        }
      }
    }
    if (!exec_status)
    {
      char **commands = malloc(1000 * sizeof(char *));
      reduce(commands, rule);
      replace(commands, makefile);
      if (commands)
      {
        for (int i = 0; commands[i] != NULL; i++)
        {
          printf("%s", commands[i]);
          if (commands[i+1] != NULL)
            printf(" ");
        }
        printf("\n");
        execvp(commands[0], commands);
        return 0;
      }
    }
    return 2;
  }
  waitpid(child_pid, &status, WUNTRACED);
  return 2;
}
