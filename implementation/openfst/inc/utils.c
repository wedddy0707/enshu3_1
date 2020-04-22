#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

FILE* fopen_with_errmsg(const char* path,const char* mode)
{
  FILE* fp;

  if((fp = fopen(path,mode)) < 0) {
    fprintf(stderr,"Failed to Open File. \"%s\" with mode \"%s\"\n", path, mode);
    exit(EXIT_FAILURE);
  }

  return fp;
}
