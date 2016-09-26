//
// Created by Yanke Guo on 2016/9/22.
//

#define __SYLVA_SOURCE__
#include "sylva/runtime.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sylva/runtime.h>

sylva_object_ref sylva_object_retain(sylva_object_ref object) {
  object->ref_count++;
  return object;
}

sylva_object_ref sylva_object_release(sylva_object_ref object) {
  if (object->ref_count <= 1) {
    object->ref_count = 0;
    sylva_object_destroy(object);
    return NULL;
  } else {
    object->ref_count--;
    return object;
  }
}

void sylva_class_calculate_instance_members_count(sylva_class class, sylva_index *out) {
  if (class.instance_member_list != NULL) {
    *out += class.instance_member_list->length;
  }
  if (class.modules != NULL) {
    for (sylva_index i = 0; i < class.modules_count; i++) {
      sylva_module_ref module = class.modules[i];
      if (module->instance_member_list != NULL) {
        *out += module->instance_member_list->length;
      }
    }
  }
  if (class.super != NULL) {
    sylva_class_calculate_instance_members_count(*class.super, out);
  }
}

void sylva_class_init_instance_members(sylva_class class, sylva_members_ref members, sylva_index start) {
  sylva_index i;
  if (class.instance_member_list != NULL) {
    for (i = 0; i < class.instance_member_list->length; i++) {
      members->member_ids[start + i] = class.instance_member_list->member_ids[i];
      members->member_options[start + i] = class.instance_member_list->member_options[i];
    }
    start += i;
  }
  for (sylva_index j = 0; j < class.modules_count; j++) {
    sylva_module_ref module = class.modules[j];
    if (module->instance_member_list != NULL) {
      for (i = 0; i < module->instance_member_list->length; i++) {
        members->member_ids[start + i] = module->instance_member_list->member_ids[i];
        members->member_options[start + i] = module->instance_member_list->member_options[i];
      }
      start += i;
    }
  }
  if (class.super != NULL) {
    sylva_class_init_instance_members(*class.super, members, start);
  }
}

sylva_object_ref sylva_object_create(sylva_class_ref class) {
  sylva_object_ref object = malloc(sizeof(sylva_object));
  object->class = class;
  //  Calculate members.length
  sylva_index members_length = 0;
  sylva_class_calculate_instance_members_count(*class, &members_length);
  //  Alloc
  object->members = sylva_members_create(members_length);
  //  Assign options and member_ids
  sylva_class_init_instance_members(*class, object->members, 0);
  //  ref_count
  object->ref_count = 0;
  //  Iterating class tree
  return object;
}

void sylva_object_destroy(sylva_object_ref object) {
  //TODO: add custom deallocator
  if (object->members != NULL) {
    sylva_members_ref members = object->members;
    object->members = NULL;
    sylva_members_destroy(members);
  }
  free(object);
}

void sylva_retain(sylva_value_ref value) {
  if (value->type == sylva_value_type_object) {
    sylva_object_retain(value->object_value);
  }
}

void sylva_release(sylva_value_ref value) {
  if (value->type == sylva_value_type_object) {
    sylva_object_ref result = sylva_object_release(value->object_value);
    if (result == NULL) {
      value->type = sylva_value_type_nil;
      value->integer_value = 0;
    }
  }
}

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

  args.length = length;
  args.values = malloc(sizeof(sylva_value) * length);

  for (sylva_index i = 0; i < length; i++) {
    sylva_value value = va_arg(list, sylva_value);
    sylva_retain(&value);
    args.values[i] = value;
  }

  return args;
}

void sylva_args_destroy(sylva_args args) {
  for (sylva_index i = args.length; i < args.length; i++) {
    sylva_release(&args.values[i]);
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
  sylva_args args = sylva_args_v_create(length, list);
  sylva_value result = func(context, args);
  sylva_args_destroy(args);
  return result;
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
  sylva_func func = NULL;

  if (class.instance_func_list != NULL) {
    func = sylva_func_list_get(*class.instance_func_list, func_id);
    if (func != NULL) {
      return func;
    }
  }
  // search module instance func
  for (sylva_index i = 0; i < class.modules_count; i++) {
    sylva_module_ref module = class.modules[i];
    if (module->instance_func_list != NULL) {
      func = sylva_func_list_get(*(module->instance_func_list), func_id);
      if (func != NULL) {
        return func;
      }
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
  sylva_func func = NULL;

  if (class.static_func_list != NULL) {
    func = sylva_func_list_get(*class.static_func_list, func_id);
    if (func != NULL) {
      return func;
    }
  }
  // search module instance func
  for (sylva_index i = 0; i < class.modules_count; i++) {
    sylva_module_ref module = class.modules[i];
    if (module->static_func_list != NULL) {
      func = sylva_func_list_get(*(module->static_func_list), func_id);
      if (func != NULL) {
        return func;
      }
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

sylva_func sylva_super_func_resolve(sylva_value context, sylva_func_id func_id) {
  if (context.type == sylva_value_type_object) {
    sylva_class_ref class = context.object_value->class->super;
    if (class != NULL)
      return sylva_class_instance_func_resolve(*class, func_id);
  }
  if (context.type == sylva_value_type_class) {
    sylva_class_ref class = context.class_value->super;
    if (class != NULL)
      return sylva_class_static_func_resolve(*context.class_value->super, func_id);
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

sylva_value sylva_super_call(sylva_value context, sylva_func_id func_id, sylva_index length, ...) {
  va_list list;
  va_start(list, length);
  sylva_value result = sylva_v_call(context, func_id, length, list);
  va_end(list);
  return result;
}

sylva_value sylva_super_v_call(sylva_value context, sylva_func_id func_id, sylva_index length, va_list list) {
  sylva_func func = sylva_super_func_resolve(context, func_id);
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

sylva_value sylva_members_get(sylva_members members, sylva_member_id member_id) {
  for (sylva_index i = 0; i < members.length; i++) {
    if (members.member_ids[i] == member_id) {
      return members.member_values[i];
    }
  }
  return sylva_value_nil;
}

sylva_value sylva_class_members_get(sylva_class class, sylva_member_id member_id) {
  return sylva_members_get(*class.members, member_id);
}

sylva_value sylva_object_members_get(sylva_object object, sylva_member_id member_id) {
  return sylva_members_get(*object.members, member_id);
}

sylva_value sylva_value_members_get(sylva_value value, sylva_member_id member_id) {
  if (value.type == sylva_value_type_object) {
    return sylva_object_members_get(*value.object_value, member_id);
  }
  if (value.type == sylva_value_type_class) {
    return sylva_class_members_get(*value.class_value, member_id);
  }
  return sylva_value_nil;
}

sylva_boolean sylva_members_set(sylva_members members, sylva_member_id member_id, sylva_value value) {
  for (sylva_index i = 0; i < members.length; i++) {
    if (members.member_ids[i] == member_id) {
      //  if this is not a weak member, release the old value and retain the new value
      if (!(members.member_options[i] & sylva_member_weak)) {
        sylva_release(&members.member_values[i]);
        sylva_retain(&value);
      }
      members.member_values[i] = value;
      return sylva_true;
    }
  }
  return sylva_false;
}

sylva_boolean sylva_class_members_set(sylva_class class, sylva_member_id member_id, sylva_value value) {
  return sylva_members_set(*class.members, member_id, value);
}

sylva_boolean sylva_object_members_set(sylva_object object, sylva_member_id member_id, sylva_value value) {
  return sylva_members_set(*object.members, member_id, value);
}

sylva_boolean sylva_value_members_set(sylva_value target_value,
                                      sylva_member_id member_id,
                                      sylva_value value) {
  if (target_value.type == sylva_value_type_object) {
    return sylva_object_members_set(*target_value.object_value, member_id, value);
  }
  if (target_value.type == sylva_value_type_class) {
    return sylva_class_members_set(*target_value.class_value, member_id, value);
  }
  return sylva_false;
}

sylva_members_ref sylva_members_create(sylva_index length) {
  //  alloc
  sylva_members_ref members = malloc(sizeof(sylva_members));
  members->length = length;
  members->member_ids = malloc(sizeof(sylva_member_id) * length);
  members->member_options = malloc(sizeof(sylva_member_option) * length);
  members->member_values = malloc(sizeof(sylva_value) * length);
  //  clear
  for (sylva_index i = 0; i < length; i++) {
    members->member_ids[i] = 0;
    members->member_options[i] = sylva_member_normal;
    members->member_values[i] = sylva_value_nil;
  }
  return members;
}

sylva_index sylva_members_partial_init(sylva_members_ref members,
                                       sylva_index start_idx,
                                       sylva_member_list member_list) {
  for (sylva_index i = 0; i < member_list.length; i++) {
    members->member_ids[start_idx + i] = member_list.member_ids[i];
    members->member_options[start_idx + 1] = member_list.member_options[i];
  }
  return member_list.length;
}

void sylva_members_destroy(sylva_members_ref members) {
  for (sylva_index i = 0; i < members->length; i++) {
    if (!(members->member_options[i] & sylva_member_weak)) {
      sylva_release(&members->member_values[i]);
    }
  }
  free(members->member_ids);
  free(members->member_options);
  free(members->member_values);
  free(members);
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
