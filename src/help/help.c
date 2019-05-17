#include "help.h"

void help(void)
{
    puts("Usage: ./minimake [options] [target] ...");
    puts("Options:");
    puts(" -f FILE    Read FILE as a makefile.");
    puts(" -h         Print this message and exit.");
}

void nofile(char *file)
{
    fprintf(stderr, "minimake: %s: No such file or directory\n", file);
}

void notarget(char *target)
{
    fprintf(stderr, "minimake: *** No rule to make target '%s'.  Stop.\n",
            target);
}

void noargument(char *arg)
{
    fprintf(stderr, "minimake: option requires an argument -- '%s'\n", arg);
}

void nomakefile(void)
{
    fprintf(stderr, "minimake: *** No targets specified and no makefile %s",
            "found.  Stop.\n");
}

void notarget_dependency(char *dependency, char *target)
{

    fprintf(stderr, "minimake: *** No rule to make target '%s', %s%s%s",
            dependency, "needed by '", target, "'.  Stop.\n");
}
