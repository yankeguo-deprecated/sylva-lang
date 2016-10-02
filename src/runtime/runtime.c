//
// Created by Yanke Guo on 2016/9/22.
//

#define __SYLVA_SOURCE__

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sylva/runtime.h>
#include <sylva/foundation.h>

sylva_class_ref sylva_get_class(sylva_value value) {
  if (value.type == sylva_type_boolean ||
      value.type == sylva_type_integer ||
      value.type == sylva_type_float ||
      value.type == sylva_type_nil) {
    return &SYLVA_Number;
  }
  if (value.type == sylva_type_object) {
    return value.object_value->class;
  }
  if (value.type == sylva_type_class) {
    return value.class_value;
  }
  return NULL;
}

sylva_imp sylva_class_instance_func_resolve(sylva_class class, sylva_symbol name) {
  // search from instance_funcs
  if (class.instance_funcs != NULL) {
    sylva_imp func = sylva_funcs_get(*class.instance_funcs, name);
    if (func != NULL) {
      return func;
    }
  }

  // walk to super
  if (class.super != NULL) {
    return sylva_class_instance_func_resolve(*class.super, name);
  }

  return NULL;
}

sylva_imp sylva_class_static_func_resolve(sylva_class class, sylva_symbol name) {
  // search from static_funcs
  if (class.static_funcs != NULL) {
    sylva_imp func = sylva_funcs_get(*class.static_funcs, name);
    if (func != NULL) {
      return func;
    }
  }

  // walk to super
  if (class.super != NULL) {
    return sylva_class_instance_func_resolve(*class.super, name);
  }
  return NULL;
}

sylva_imp sylva_func_resolve(sylva_value context, sylva_symbol name) {
  sylva_class_ref class = sylva_get_class(context);

  if (class == NULL) {
    return NULL;
  }

  if (context.type == sylva_type_class) {
    return sylva_class_static_func_resolve(*class, name);
  } else {
    return sylva_class_instance_func_resolve(*class, name);
  }
}

sylva_imp sylva_func_resolve_super(sylva_value context, sylva_class_ref class, sylva_symbol name) {
  //  super can only be called on object or class
  assert(context.type == sylva_type_object || context.type == sylva_type_class);

  if (context.type == sylva_type_object) {
    if (class != NULL && class->super != NULL)
      return sylva_class_instance_func_resolve(*class->super, name);
  }
  if (context.type == sylva_type_class) {
    if (class != NULL && class->super != NULL)
      return sylva_class_static_func_resolve(*class->super, name);
  }
  return NULL;
}

sylva_value sylva_call(sylva_value context, sylva_symbol name, sylva_index length, ...) {
  va_list list;
  va_start(list, length);
  sylva_value result = sylva_v_call(context, name, length, list);
  va_end(list);
  return result;
}

SYLVA_EXTERN sylva_value sylva_v_call(sylva_value context, sylva_symbol name, sylva_index length, va_list list) {
  sylva_imp func = sylva_func_resolve(context, name);
  if (func != NULL) {
    return sylva_func_v_call(func, context, length, list);
  }
  return sylva_nil_value;
}

sylva_value sylva_call_super(sylva_value context,
                             sylva_class_ref class,
                             sylva_symbol name,
                             sylva_index length,
                             ...) {
  va_list list;
  va_start(list, length);
  sylva_value result = sylva_v_call_super(context, class, name, length, list);
  va_end(list);
  return result;
}

sylva_value sylva_v_call_super(sylva_value context,
                               sylva_class_ref class,
                               sylva_symbol name,
                               sylva_index length,
                               va_list list) {
  sylva_imp func = sylva_func_resolve_super(context, class, name);
  if (func != NULL) {
    return sylva_func_v_call(func, context, length, list);
  }
  return sylva_nil_value;
}

sylva_value sylva_class_members_get(sylva_class class, sylva_symbol name) {
  return sylva_members_get(*class.members, name);
}

sylva_value sylva_object_members_get(sylva_object object, sylva_symbol name) {
  return sylva_members_get(*object.members, name);
}

sylva_boolean sylva_class_members_set(sylva_class class, sylva_symbol name, sylva_value value) {
  return sylva_members_set(*class.members, name, value);
}

sylva_boolean sylva_object_members_set(sylva_object object, sylva_symbol name, sylva_value value) {
  return sylva_members_set(*object.members, name, value);
}

sylva_value sylva_get(sylva_value value, sylva_symbol name) {
  if (value.type == sylva_type_object) {
    return sylva_object_members_get(*value.object_value, name);
  }
  if (value.type == sylva_type_class) {
    return sylva_class_members_get(*value.class_value, name);
  }
  return sylva_nil_value;
}

sylva_value sylva_static_get(sylva_value value, sylva_symbol name) {
  if (value.type == sylva_type_object) {
    value = sylva_class_value(value.object_value->class);
  }
  return sylva_get(value, name);
}

sylva_boolean sylva_set(sylva_value target_value, sylva_symbol name, sylva_value value) {
  if (target_value.type == sylva_type_object) {
    return sylva_object_members_set(*target_value.object_value, name, value);
  }
  if (target_value.type == sylva_type_class) {
    return sylva_class_members_set(*target_value.class_value, name, value);
  }
  return sylva_false;
}

sylva_boolean sylva_static_set(sylva_value target_value, sylva_symbol name, sylva_value value) {
  if (target_value.type == sylva_type_object) {
    target_value = sylva_class_value(target_value.object_value->class);
  }
  return sylva_set(target_value, name, value);
}
