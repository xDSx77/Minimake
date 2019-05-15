#pragma once
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct var
{
  char *name;
  char *data_c;
  char **data;
};

struct rule
{
  char *target;
  char **dependencies_c;
  struct rule **dependencies;
  char *commands_c;
  char **commands;
};


struct makefile
{
  struct var **vars;
  struct rule **rules;
};

struct makefile *create_struct(FILE *file, int lines);
