#ifndef EXECUTION_H
#define EXECUTION_H

#include <unistd.h>
#include <sys/wait.h>
#include "../makefile/makefile.h"

char *extract(char *command);

void reduce(char **commands, struct rule *rule);

void replace(char **commands, struct makefile *makefile);

char **separate(char **commands);

int execute(struct makefile *makefile, struct rule *rule);

#endif /* ! EXECUTION_H */
