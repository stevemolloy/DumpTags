#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef enum {
  STATE_SKIPPING = 0,
} State;

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

char *tok;

int main(void)
{
  const char *filename = "./dump_info.txt";
  char *contents;

  read_entire_file(filename, &contents);

  tok = strtok(contents, " \n");
  while (tok != NULL)
  {
    printf("%s\n", tok);
    tok = strtok(NULL, " \n");
  }

  free(contents);

  return 0;
}
