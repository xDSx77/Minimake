#include <unistd.h>
#include <sys/wait.h>
#include "../makefile/makefile.h"

char *extract(char *command);

void execute(struct makefile *makefile, struct rule *rule);
