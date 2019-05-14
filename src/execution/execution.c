#include "execution.h"

void execute(struct rule *rule)
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
    execvp(rule->commands[0],
      rule->commands + strlen(rule->commands[0]) + 1);
  }
  else
    waitpid(child_pid, &status, WUNTRACED);
}
