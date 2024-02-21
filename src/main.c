#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "json.h"

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
  const char *filename = "./R3_B080603_CAB02_VAC_PLC01.json";
  // const char *filename = "./rickandmorty.json";
  char *contents;

  read_entire_file(filename, &contents);

  result(json_element) element_result = json_parse(contents);

  if (result_is_err(json_element)(&element_result)) {
    typed(json_error) error = result_unwrap_err(json_element)(&element_result);
    fprintf(stderr, "Error parsing JSON: %s\n", json_error_to_string(error));
    return -1;
  }
  typed(json_element) element = result_unwrap(json_element)(&element_result);

  json_print(&element, 2);
  json_free(&element);

  free(contents);

  return 0;
}
