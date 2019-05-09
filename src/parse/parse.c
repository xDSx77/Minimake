#include "parse.h"

void parse(struct makefile *makefile, FILE *file)
{
  char *line = "";
  size_t len = strlen(line);
  bool is_a_rule = false;
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
      if (commands != NULL)
        for (int i = 0; commands[i] != NULL; i++)
          puts(commands[i]);
      continue;
    }
    for (size_t i = 0; i < strlen(line); i++)
    {
      if (line[i] == ' ' || line[i] == '\n')
        continue;
      else if (line[i] == '#')
        break;
      else if (line[i] == '=')
        add_var(makefile, line);
      else if (line[i] == ':')
      {
        add_rule(makefile, line);
        is_a_rule = true;
      }
    }
  }
}

void add_var(struct makefile *makefile, char *line)
{
  char *name = strtok(line, "=");
  puts(name);
  puts("=");
  char *data_l = strtok(NULL, "\n#");
  char **data = split(data_l);
  if (data != NULL)
    for (int i = 0; data[i] != NULL; i++)
      puts(data[i]);
  makefile = makefile;
}

void add_rule(struct makefile *makefile, char *line)
{
  char *target = strtok(line, ":");
  puts(target);
  puts(":");
  char *dependencies_l = strtok(NULL, "\n#");
  char **dependencies = split(dependencies_l);
  if (dependencies != NULL)
    for (int i = 0; dependencies[i] != NULL; i++)
      puts(dependencies[i]);
  makefile = makefile;
}

char **split(char *data_l)
{
  if (data_l == NULL)
    return NULL;
  char **data = malloc(strlen(data_l) * sizeof(char*));
  int i = 0;
  char *token = strtok(data_l, " \t");
  while (token)
  {
    data[i] = token;
    token = strtok(NULL, " \t\n");
    i++;
  }
  return data;
}
