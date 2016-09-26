//
// Created by Yanke Guo on 2016/9/22.
//

#define __SYLVA_SOURCE__
#include "sylva/runtime.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sylva/runtime.h>

sylva_func sylva_class_instance_func_resolve(sylva_class class, sylva_func_id func_id) {
  // search from instance_funcs
  if (class.instance_funcs != NULL) {
    sylva_func func = sylva_funcs_get(*class.instance_funcs, func_id);
    if (func != NULL) {
      return func;
    }
  }

  // walk to super
  if (class.super != NULL) {
    return sylva_class_instance_func_resolve(*class.super, func_id);
  }

  return NULL;
}

sylva_func sylva_class_static_func_resolve(sylva_class class, sylva_func_id func_id) {
  // search from static_funcs
  if (class.static_funcs != NULL) {
    sylva_func func = sylva_funcs_get(*class.static_funcs, func_id);
    if (func != NULL) {
      return func;
    }
  }

  // walk to super
  if (class.super != NULL) {
    return sylva_class_instance_func_resolve(*class.super, func_id);
  }
  return NULL;
}

sylva_func sylva_func_resolve(sylva_value context, sylva_func_id func_id) {
  if (context.type == sylva_type_object) {
    return sylva_class_instance_func_resolve(*context.object_value->class, func_id);
  }
  if (context.type == sylva_type_class) {
    return sylva_class_static_func_resolve(*context.class_value, func_id);
  }
  return NULL;
}

sylva_func sylva_func_resolve_super(sylva_value context, sylva_class_ref class, sylva_func_id func_id) {
  if (context.type == sylva_type_object) {
    if (class != NULL && class->super != NULL)
      return sylva_class_instance_func_resolve(*class->super, func_id);
  }
  if (context.type == sylva_type_class) {
    if (class != NULL && class->super != NULL)
      return sylva_class_static_func_resolve(*class->super, func_id);
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

sylva_value sylva_call_super(sylva_value context,
                             sylva_class_ref class,
                             sylva_func_id func_id,
                             sylva_index length,
                             ...) {
  va_list list;
  va_start(list, length);
  sylva_value result = sylva_v_call_super(context, class, func_id, length, list);
  va_end(list);
  return result;
}

sylva_value sylva_v_call_super(sylva_value context,
                               sylva_class_ref class,
                               sylva_func_id func_id,
                               sylva_index length,
                               va_list list) {
  sylva_func func = sylva_func_resolve_super(context, class, func_id);
  if (func != NULL) {
    return sylva_func_v_call(func, context, length, list);
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
  if (value.type == sylva_type_object) {
    return sylva_object_members_get(*value.object_value, member_id);
  }
  if (value.type == sylva_type_class) {
    return sylva_class_members_get(*value.class_value, member_id);
  }
  return sylva_value_nil;
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
  if (target_value.type == sylva_type_object) {
    return sylva_object_members_set(*target_value.object_value, member_id, value);
  }
  if (target_value.type == sylva_type_class) {
    return sylva_class_members_set(*target_value.class_value, member_id, value);
  }
  return sylva_false;
}

