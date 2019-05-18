#ifndef MAKEFILE_H
#define MAKEFILE_H

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
    int nb_data;
};

struct rule
{
    char *target;
    char **dependencies_c;
    char ***commands;
    int *nb_commands;
    int lines;
    int nb_dependencies;
};


struct makefile
{
    struct var **vars;
    struct rule **rules;
    int nb_vars;
    int nb_rules;
};

struct makefile *create_struct(FILE *file, int lines);

#endif /* ! MAKEFILE_H */
