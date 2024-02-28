#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"
#include "lib.h"

char *tok;

int main(void)
{
  // const char *filename = "./R3_B080603_CAB02_VAC_PLC01.json";
  const char *filename = "./R3_B080603_CAB02_VAC_PLC02.json";
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

