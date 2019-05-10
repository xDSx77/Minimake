#pragma once
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

struct var
{
  char *name;
  char **data;
};

struct rule
{
  char *target;
  struct rule *dependencies;
  char **commands;
};


struct makefile
{
  struct var **vars;
  struct rule **rules;
};

struct makefile *create_struct(FILE *file);
