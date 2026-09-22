#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lib.h"
#include "lexer.h"
#include "parser.h"

#define NOB_IMPLEMENTATION
#include "nob.h"

int main(void) {
  const char *filename = "./R3_B080603_CAB02_VAC_PLC01.json";
  // const char *filename = "./R3_B080603_CAB02_VAC_PLC02.json";
  String_Builder contents = {0};

  read_entire_file(filename, &contents);

  result(json_element) top_level_result = json_parse(contents.items);
  if (result_is_err(json_element)(&top_level_result)) {
    typed(json_error) error = result_unwrap_err(json_element)(&top_level_result);
    fprintf(stderr, "Error parsing JSON: %s\n", json_error_to_string(error));
    return -1;
  }
  json_element_t top_level_element = result_unwrap(json_element)(&top_level_result);

  json_element_t main_rfdump_tag_ele = get_sub_element(
      get_sub_element(top_level_element, "rfdump"),
      "B_R3_VAC_PLC01_RFDMP_LOCAL_HB"
  );

  json_element_t rfdump_conditions_ele = get_sub_element(main_rfdump_tag_ele, "conditions");

  size_t string_len = strlen(rfdump_conditions_ele.value.as_string) + 1;
  char *string_to_parse = calloc(string_len, sizeof(char));
  memcpy(string_to_parse, rfdump_conditions_ele.value.as_string, string_len);

  printf("%s\n", string_to_parse);

  String_View stmt_sv = sv_from_cstr(string_to_parse);

  Tokens tokens = lex_string_view(stmt_sv);
  Node *root = parse_statement(&tokens);
  if (root == NULL) return 1;
  print_node(root, 0);

  String_View_List signals = {0};
  extract_signal_names_from_tree(root, &signals);

  printf("The following signals were found in the tree:\n");
  da_foreach(String_View, signal, &signals) {
      printf("\t" SV_Fmt "\n", SV_Arg(*signal));
  }

  free(string_to_parse);
  json_free(&top_level_element);
  free(contents.items);

  return 0;
}
