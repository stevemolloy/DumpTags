#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "json.h"
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

typed(json_element) get_sub_element(json_element_t haystack, const char *needle)
{
  assert(haystack.type == JSON_ELEMENT_TYPE_OBJECT && "`haystack` needs to be a JSON obj for this to work");
  result(json_element) element_result = json_object_find(haystack.value.as_object, needle);
  if (result_is_err(json_element)(&element_result))
  {
    typed(json_error) error = result_unwrap_err(json_element)(&element_result);
    fprintf(stderr, "Error parsing JSON: %s\n", json_error_to_string(error));
    exit(-1);
  }
  return result_unwrap(json_element)(&element_result);
}

char* json_ele_type_to_string(json_element_t ele)
{
  switch (ele.type)
  {
    case JSON_ELEMENT_TYPE_NULL:
      return "NULL";
    case JSON_ELEMENT_TYPE_ARRAY:
      return "ARRAY";
    case JSON_ELEMENT_TYPE_NUMBER:
      return "NUMBER";
    case JSON_ELEMENT_TYPE_STRING:
      return "STRING";
    case JSON_ELEMENT_TYPE_OBJECT:
      return "OBJECT";
    case JSON_ELEMENT_TYPE_BOOLEAN:
      return "BOOLEAN";
  }
}

