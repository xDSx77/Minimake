#ifndef EXECUTION_H
#define EXECUTION_H

#include <unistd.h>
#include <sys/wait.h>
#include "../makefile/makefile.h"

char *extract(char *command);

void reduce(char **commands, struct rule *rule, int rule_idx);

void replace(char **commands, struct makefile *makefile);

char **separate(char **commands);

int execute(struct makefile *makefile, struct rule *rule);

void print(char **commands);

#endif /* ! EXECUTION_H */
