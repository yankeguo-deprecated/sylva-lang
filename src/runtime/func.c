//
// Created by Ryan on 2016/9/26.
//

#include "sylva/runtime.h"

#include <stdlib.h>
#include <string.h>

sylva_value sylva_func_call(sylva_func func, sylva_value context, sylva_index length, ...) {
  va_list list;
  va_start(list, length);
  sylva_value result = sylva_func_v_call(func, context, length, list);
  va_end(list);
  return result;
}

sylva_value sylva_func_v_call(sylva_func func, sylva_value context, sylva_index length, va_list list) {
  sylva_args args = sylva_args_v_create(length, list);
  sylva_value result = func(context, args);
  sylva_args_destroy(args);
  return result;
}

sylva_boolean sylva_funcs_exists(sylva_funcs list, sylva_func_id func_id) {
  for (sylva_index i = 0; i < list.length; i++) {
    if (list.func_ids[i] == func_id) {
      return sylva_true;
    }
  }
  return sylva_false;
}

sylva_func sylva_funcs_get(sylva_funcs list, sylva_func_id func_id) {
  for (sylva_index i = 0; i < list.length; i++) {
    if (list.func_ids[i] == func_id)
      return list.funcs[i];
  }
  return NULL;
}

sylva_func sylva_funcs_set(sylva_funcs list, sylva_func_id func_id, sylva_func func) {
  for (sylva_index i = 0; i < list.length; i++) {
    if (list.func_ids[i] == func_id || list.func_ids[0] == 0) {
      list.func_ids[i] = func_id;
      list.funcs[i] = func;
    }
  }
  return NULL;
}

char *sylva_func_id_registry_get_func_name(sylva_func_id_registry registry, sylva_func_id func_id) {
  for (sylva_index i = 0; i < registry.length; i++) {
    if (registry.func_ids[i] == func_id) {
      return registry.func_names[i];
    }
  }
  return NULL;
}

sylva_func_id sylva_func_id_registry_get_func_id(sylva_func_id_registry registry, char *func_name) {
  for (sylva_index i = 0; i < registry.length; i++) {
    if (strcmp(registry.func_names[i], func_name) == 0) {
      return registry.func_ids[i];
    }
  }
  return sylva_func_id_not_found;
}

char *sylva_func_id_to_name(sylva_func_id func_id) {
  return sylva_func_id_registry_get_func_name(sylva_runtime_func_id_registry, func_id);
}

sylva_func_id sylva_func_name_to_id(char *func_name) {
  return sylva_func_id_registry_get_func_id(sylva_runtime_func_id_registry, func_name);
}
