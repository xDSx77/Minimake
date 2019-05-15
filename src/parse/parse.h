#pragma once
#include "../makefile/makefile.h"

int parse(struct makefile *makefile, FILE *file);

int add_var(struct makefile *makefile, char *line);

int add_rule(struct makefile *makefile, char *line, int *rule_idx);

char **split(char *data);
