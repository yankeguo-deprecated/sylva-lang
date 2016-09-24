//
// Created by Yanke Guo on 2016/9/22.
//

#define __SYLVA_SOURCE__
#include "sylva/runtime.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

sylva_value sylva_args_get(sylva_args args, sylva_index index) {
  if (args.length > index) {
    return args.values[index];
  }
  return (sylva_value) {
      .type = sylva_value_type_nil,
      .integer_value = 0,
  };
}

SYLVA_EXTERN sylva_args sylva_args_create(sylva_index length, ...) {
  va_list list;
  va_start(list, length);
  sylva_args args = sylva_args_v_create(length, list);
  va_end(list);
  return args;
}

sylva_args sylva_args_v_create(sylva_index length, va_list list) {
  sylva_args args = sylva_args_empty;

  if (length > 0) {
    args.values = malloc(sizeof(sylva_value) * length);
    for (sylva_index i = 0; i < length; i++) {
      args.values[i] = va_arg(list, sylva_value);
    }
  }

  return args;
}

void sylva_args_destroy(sylva_args args) {
  if (args.values != NULL) {
    free(args.values);
  }
}

sylva_value sylva_func_call(sylva_func func, sylva_value context, sylva_index length, ...) {
  va_list list;
  va_start(list, length);
  sylva_value result = sylva_func_v_call(func, context, length, list);
  va_end(list);
  return result;
}

sylva_value sylva_func_v_call(sylva_func func, sylva_value context, sylva_index length, va_list list) {
  return func(context, sylva_args_v_create(length, list));
}

sylva_boolean sylva_func_list_exists(sylva_func_list list, sylva_func_id func_id) {
  for (sylva_index i = 0; i < list.length; i++) {
    if (list.func_ids[i] == func_id) {
      return sylva_true;
    }
  }
  return sylva_false;
}

sylva_func sylva_func_list_get(sylva_func_list list, sylva_func_id func_id) {
  for (sylva_index i = 0; i < list.length; i++) {
    if (list.func_ids[i] == func_id)
      return list.funcs[i];
  }
  return NULL;
}

sylva_func sylva_func_list_set(sylva_func_list list, sylva_func_id func_id, sylva_func func) {
  for (sylva_index i = 0; i < list.length; i++) {
    if (list.func_ids[i] == func_id || list.func_ids[0] == 0) {
      list.func_ids[i] = func_id;
      list.funcs[i] = func;
    }
  }
  return NULL;
}

sylva_func sylva_class_instance_func_resolve(sylva_class class, sylva_func_id func_id) {
  // search class instance func
  sylva_func instance_func = sylva_func_list_get(*class.instance_func_list, func_id);
  if (instance_func != NULL) {
    return instance_func;
  }
  // search module instance func
  for (sylva_index i = 0; i < class.modules_count; i++) {
    sylva_module module = class.modules[i];
    instance_func = sylva_func_list_get(*module.instance_func_list, func_id);
    if (instance_func != NULL) {
      return instance_func;
    }
  }
  // walk to super
  if (class.super != NULL) {
    return sylva_class_instance_func_resolve(*class.super, func_id);
  }
  return NULL;
}

sylva_func sylva_class_static_func_resolve(sylva_class class, sylva_func_id func_id) {
  // search class instance func
  sylva_func instance_func = sylva_func_list_get(*class.static_func_list, func_id);
  if (instance_func != NULL) {
    return instance_func;
  }
  // search module instance func
  for (sylva_index i = 0; i < class.modules_count; i++) {
    sylva_module module = class.modules[i];
    instance_func = sylva_func_list_get(*module.static_func_list, func_id);
    if (instance_func != NULL) {
      return instance_func;
    }
  }
  // walk to super
  if (class.super != NULL) {
    return sylva_class_instance_func_resolve(*class.super, func_id);
  }
  return NULL;
}

sylva_func sylva_func_resolve(sylva_value context, sylva_func_id func_id) {
  if (context.type == sylva_value_type_object) {
    return sylva_class_instance_func_resolve(*context.object_value->class, func_id);
  }
  if (context.type == sylva_value_type_class) {
    return sylva_class_static_func_resolve(*context.class_value, func_id);
  }
  return NULL;
}

sylva_value sylva_call(sylva_value context, sylva_func_id func_id, sylva_index length, ...) {
  va_list list;
  va_start(list, length);
  sylva_value result = sylva_v_call(context, func_id, length, list);
  va_end(list);
  return result;
}

SYLVA_EXTERN sylva_value sylva_v_call(sylva_value context, sylva_func_id func_id, sylva_index length, va_list list) {
  sylva_func func = sylva_func_resolve(context, func_id);
  if (func != NULL) {
    return sylva_func_v_call(func, context, length, list);
  }
  return sylva_value_nil;
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

char *sylva_runtime_func_id_to_name(sylva_func_id func_id) {
  return sylva_func_id_registry_get_func_name(sylva_runtime_func_id_registry, func_id);
}

sylva_func_id sylva_runtime_func_name_to_id(char *func_name) {
  return sylva_func_id_registry_get_func_id(sylva_runtime_func_id_registry, func_name);
}

char *sylva_member_id_registry_get_func_name(sylva_member_id_registry registry, sylva_member_id member_id) {
  for (sylva_index i = 0; i < registry.length; i++) {
    if (registry.member_ids[i] == member_id) {
      return registry.member_names[i];
    }
  }
  return NULL;
}

sylva_member_id sylva_member_id_registry_get_func_id(sylva_member_id_registry registry, char *member_name) {
  for (sylva_index i = 0; i < registry.length; i++) {
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
