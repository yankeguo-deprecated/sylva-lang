//
// Created by Yanke Guo on 2016/9/22.
//

#include "sylva/runtime.h"

#include <stdlib.h>
#include <string.h>
#include <sylva/runtime.h>

sylva_boolean sylva_func_list_exists(sylva_func_list list, sylva_func_id func_id) {
  for (int i = 0; i < list.capacity; i++) {
    if (list.func_ids[i] == func_id) {
      return sylva_true;
    }
  }
  return sylva_false;
}

sylva_func sylva_func_list_get(sylva_func_list list, sylva_func_id func_id) {
  for (int i = 0; i < list.capacity; i++) {
    if (list.func_ids[i] == func_id)
      return list.funcs[i];
  }
  return NULL;
}

sylva_func sylva_func_list_set(sylva_func_list list, sylva_func_id func_id, sylva_func func) {
  for (int i = 0; i < list.capacity; i++) {
    if (list.func_ids[i] == func_id || list.func_ids[0] == 0) {
      list.func_ids[i] = func_id;
      list.funcs[i] = func;
    }
  }
  return NULL;
}
