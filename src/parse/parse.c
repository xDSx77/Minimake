#include "parse.h"

int parse(struct makefile *makefile, FILE *file)
{
  char *line = "";
  size_t len = strlen(line);
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
      makefile->rules[cur_rule_idx]->commands = commands;
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
  //add_dependencies(makefile);
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
  makefile->vars[i]->name = malloc(strlen(name));
  strncpy(makefile->vars[i]->name, name, strlen(name));
  for (int j = 0; data[j] != NULL; j++)
  {
    makefile->vars[i]->data[j] = malloc(strlen(data[j]));
    strncpy(makefile->vars[i]->data[j], data[j], strlen(data[j]));
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
  while (makefile->rules[i]->target != NULL)
    i++;
  *rule_idx = i;
  makefile->rules[i]->target = malloc(strlen(target));
  strncpy(makefile->rules[i]->target, target, strlen(target));
  if (dependencies != NULL)
  {
    for (int j = 0; dependencies[j] != NULL; j++)
    {
      makefile->rules[i]->dependencies_c[j] = malloc(strlen(dependencies[j]));
      strncpy(makefile->rules[i]->dependencies_c[j], dependencies[j],
        strlen(dependencies[j]));
    }
    free(dependencies);
  }
  else
    makefile->rules[i]->dependencies_c = NULL;
  return 0;
}

/*void add_dependencies(struct makefile *makefile)
{
  for (int i = 0; makefile->rules[i] != NULL; i++)
  {
    if (makefile->rules[i]->dependencies_c)
      for (int j = 0; makefile->rules[i]->dependencies_c[j] != NULL; j++)
      {
        for (int k = 0; makefile->rules[k] != NULL; k++)
        {
          if (makefile->rules[i]->target && makefile->rules[k]->target)
            if (strcmp(makefile->rules[i]->target, makefile->rules[k]->target) &&
              (strcmp(makefile->rules[i]->dependencies_c[j], makefile->rules[k]->target) == 0))
            {
              makefile->rules[i]->dependencies[j] = makefile->rules[k];
            }
        }
      }
  }
}*/

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
