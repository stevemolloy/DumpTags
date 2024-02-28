#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "lib.h"

int read_entire_file(const char *filename, char **buffer)
{
  FILE *f = fopen(filename, "r");
  if (fseek(f, 0L, SEEK_END) != 0)
  {
    fprintf(stderr, "Something has gone wrong with the file\n");
    exit(1);
  }

  int file_size = ftell(f);

  if (fseek(f, 0L, SEEK_SET) != 0)
  {
    fprintf(stderr, "Something has gone wrong with the file\n");
    exit(1);
  }

  *buffer = calloc(file_size, sizeof(char));
  if (*buffer == NULL)
  {
    fprintf(stderr, "Mem alloc failed\n");
    exit(1);
  }

  size_t bytes_read = fread(*buffer, 1, file_size, f);
  assert((int)bytes_read == file_size &&
      "Num bytes read not equal to file size");

  fclose(f);

  return file_size;
}

