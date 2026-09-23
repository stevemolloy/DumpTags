#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "json.h"
#include "lib.h"
#include "lexer.h"
#include "parser.h"

#define NOB_IMPLEMENTATION
#include "nob.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef struct Implementations {
  const char *key;
  Node *value;
} Implementations;


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

  json_element_t rfdump_head = get_sub_element_unwrapped(top_level_element, "rfdump");
  assert(rfdump_head.type == JSON_ELEMENT_TYPE_OBJECT);
  
  Implementations *impls = NULL;
  json_object_t *rfdump_head_obj = rfdump_head.value.as_object;
  for (size_t i = 0; i < rfdump_head_obj->count; i++) {
    const char *key = rfdump_head_obj->entries[i]->key;
    json_element_t ele = rfdump_head_obj->entries[i]->element;
    result(json_element) conds_res = get_sub_element(ele, "conditions");
    if (result_is_err(json_element)(&conds_res)) {
	continue;
    }
    json_element_t conds = result_unwrap(json_element)(&conds_res);
    Tokens tokens = lex_string_view(sv_from_cstr(conds.value.as_string));
    Node *new_node = parse_statement(&tokens);
    simplify_node_tree(new_node);
    hmput(impls, key, new_node);
    // printf("Conditions: %s\n", conds.value.as_string);
  }

  printf("I found %ld implementations\n", hmlen(impls));
  for (size_t i = 0; i < hmlenu(impls); i++) {
    const char *key = impls[i].key;
    Node *node = impls[i].value;
    printf("Key: %s\n", key);
    print_node(node, 1);
  }

  // json_element_t main_rfdump_tag_ele = get_sub_element_unwrapped(rfdump_head, "B_R3_VAC_PLC01_RFDMP_LOCAL_HB");

  // json_element_t rfdump_conditions_ele = get_sub_element_unwrapped(main_rfdump_tag_ele, "conditions");

  // size_t string_len = strlen(rfdump_conditions_ele.value.as_string) + 1;
  // char *string_to_parse = calloc(string_len, sizeof(char));
  // memcpy(string_to_parse, rfdump_conditions_ele.value.as_string, string_len);

  // String_View stmt_sv = sv_from_cstr(string_to_parse);

  // Tokens tokens = lex_string_view(stmt_sv);
  // Node *root = parse_statement(&tokens);
  // if (root == NULL) return 1;
  // print_node(root, 0);

  // simplify_node_tree(root);
  // print_node(root, 0);
  
  // String_View_List signals = {0};
  // extract_signal_names_from_tree(root, &signals);

  // printf("The following signals were found in the tree:\n");
  // da_foreach(String_View, signal, &signals) {
  //     printf("\t" SV_Fmt "\n", SV_Arg(*signal));
  // }

  // free(string_to_parse);
  hmfree(impls);
  json_free(&top_level_element);
  free(contents.items);

  return 0;
}
