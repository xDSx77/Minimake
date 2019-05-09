#include "../makefile/makefile.h"

void parse(struct makefile *makefile, FILE *file);

void add_var(struct makefile *makefile, char *line);

void add_rule(struct makefile *makefile, char *line);

char **split(char *data);
