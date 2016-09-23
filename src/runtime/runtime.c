//
// Created by Yanke Guo on 2016/9/22.
//

#define __SYLVA_SOURCE__
#include "sylva/runtime.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

sylva_value sylva_args_get(sylva_args args, sylva_index index) {
  if (args.capacity > index) {
    return args.values[index];
  }
  return (sylva_value) {
      .type = sylva_value_type_nil,
      .integer_value = 0,
  };
}

sylva_boolean sylva_func_list_exists(sylva_func_list list, sylva_func_id func_id) {
  for (sylva_index i = 0; i < list.capacity; i++) {
    if (list.func_ids[i] == func_id) {
      return sylva_true;
    }
  }
  return sylva_false;
}

sylva_func sylva_func_list_get(sylva_func_list list, sylva_func_id func_id) {
  for (sylva_index i = 0; i < list.capacity; i++) {
    if (list.func_ids[i] == func_id)
      return list.funcs[i];
  }
  return NULL;
}

sylva_func sylva_func_list_set(sylva_func_list list, sylva_func_id func_id, sylva_func func) {
  for (sylva_index i = 0; i < list.capacity; i++) {
    if (list.func_ids[i] == func_id || list.func_ids[0] == 0) {
      list.func_ids[i] = func_id;
      list.funcs[i] = func;
    }
  }
  return NULL;
}

char *sylva_func_id_registry_get_func_name(sylva_func_id_registry registry, sylva_func_id func_id) {
  for (sylva_index i = 0; i < registry.capacity; i++) {
    if (registry.func_ids[i] == func_id) {
      return registry.func_names[i];
    }
  }
  return NULL;
}

sylva_func_id sylva_func_id_registry_get_func_id(sylva_func_id_registry registry, char *func_name) {
  for (sylva_index i = 0; i < registry.capacity; i++) {
    if (strcmp(registry.func_names[i], func_name) == 0) {
      return registry.func_ids[i];
    }
  }
  return sylva_func_id_not_found;
}

char *sylva_runtime_func_id_to_name(sylva_func_id func_id) {
  return sylva_func_id_registry_get_func_name(sylva_runtime_func_id_registry, func_id);
}

sylva_func_id sylva_runtime_func_name_to_id(char *func_name) {
  return sylva_func_id_registry_get_func_id(sylva_runtime_func_id_registry, func_name);
}

char *sylva_member_id_registry_get_func_name(sylva_member_id_registry registry, sylva_member_id member_id) {
  for (sylva_index i = 0; i < registry.capacity; i++) {
    if (registry.member_ids[i] == member_id) {
      return registry.member_names[i];
    }
  }
  return NULL;
}

sylva_member_id sylva_member_id_registry_get_func_id(sylva_member_id_registry registry, char *member_name) {
  for (sylva_index i = 0; i < registry.capacity; i++) {
    if (strcmp(registry.member_names[i], member_name) == 0) {
      return registry.member_ids[i];
    }
  }
  return sylva_member_id_not_found;
}

char *sylva_runtime_member_id_to_name(sylva_member_id member_id) {
  return sylva_member_id_registry_get_func_name(sylva_runtime_member_id_registry, member_id);
}

sylva_member_id sylva_runtime_member_name_to_id(char *member_name) {
  return sylva_member_id_registry_get_func_id(sylva_runtime_member_id_registry, member_name);
}
