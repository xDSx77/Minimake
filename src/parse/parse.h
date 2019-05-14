#pragma once
#include "../makefile/makefile.h"

int parse(struct makefile *makefile, FILE *file);

int add_var(struct makefile *makefile, char *line);

int add_rule(struct makefile *makefile, char *line, int *rule_idx);

void add_dependencies(struct makefile *makefile);

char **split(char *data);
