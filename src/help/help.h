#ifndef HELP_H
#define HELP_H

#include <stdio.h>

void help(void);

void nofile(char *file);

void notarget(char *target);

void noargument(char *arg);

void nomakefile(void);

void notarget_dependency(char *dependency, char *target);

void nothing(char *target);

#endif /* ! HELP_H */
