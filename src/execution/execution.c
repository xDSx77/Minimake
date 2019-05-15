#include "execution.h"

char *extract(char *command)
{
  char *extract = malloc(sizeof(char *));
  for (size_t i = 0; i < strlen(command) - 3; i++)
    extract[i] = command[i+2];
  return extract;
}

void execute(struct makefile *makefile, struct rule *rule)
{
  int status;
  pid_t child_pid = fork();
  if (child_pid == -1)
  {
    perror("fork");
    exit(1);
  }
  if (child_pid == 0)
  {
    char **commands = malloc(1000 * sizeof(char *));
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
    for (int i = 0; commands[i] != NULL; i++)
    {
      for (int j = 0; makefile->vars[j] != NULL; j++)
      {
        if (strcmp(commands[i], makefile->vars[j]->name) == 0)
        {
          commands[i] = "";
          for (int k = 0; makefile->vars[j]->data[k] != NULL; k++)
          {
            strncat(commands[i], makefile->vars[j]->data[k],
              strlen(makefile->vars[j]->data[k]));
            if (makefile->vars[j]->data[k+1] != NULL)
              strcat(commands[i], " ");
          }
        }
      }
    }
    if (commands)
      execvp(commands[0], commands);
  }
  else
    waitpid(child_pid, &status, WUNTRACED);
}
