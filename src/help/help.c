#include "help.h"

void help(void)
{
  puts("Usage: ./minimake [options] [target] ...");
  puts("Options:");
  puts(" -f FILE    Read FILE as a makefile.");
  puts(" -h         Print this message and exit.");
}
