#ifndef _LIB_H
#define _LIB_H

#include "json.h"

int sdm_read_entire_file(const char *filename, char **buffer);

result(json_element)
    get_sub_element(json_element_t haystack, const char *needle);
typed(json_element) get_sub_element_unwrapped(json_element_t haystack, const char *needle);

char* json_ele_type_to_string(json_element_t ele);

#endif // _LIB_H
