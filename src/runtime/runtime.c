//
// Created by Yanke Guo on 2016/9/22.
//

#define __SYLVA_RUNTIME_SOURCE__
#include <sylva/runtime.h>

sl_class_ref sl_get_class(sl_value value) {
  if (value.type == sl_type_boolean ||
      value.type == sl_type_integer ||
      value.type == sl_type_float ||
      value.type == sl_type_nil) {
    return &sl_class_name(Number);
  }
  if (value.type == sl_type_object) {
    return value.value.as_object->class;
  }
  if (value.type == sl_type_class) {
    return value.value.as_class;
  }
  return NULL;
}

sl_imp sl_class_instance_func_resolve(sl_class class, sl_symbol name) {
  // search from instance_funcs
  if (class.instance_funcs != NULL) {
    sl_imp func = sl_funcs_get(*class.instance_funcs, name);
    if (func != NULL) {
      return func;
    }
  }

  // walk to super
  if (class.super != NULL) {
    return sl_class_instance_func_resolve(*class.super, name);
  }

  return NULL;
}

sl_imp sl_class_static_func_resolve(sl_class class, sl_symbol name) {
  // search from static_funcs
  if (class.static_funcs != NULL) {
    sl_imp func = sl_funcs_get(*class.static_funcs, name);
    if (func != NULL) {
      return func;
    }
  }

  // walk to super
  if (class.super != NULL) {
    return sl_class_instance_func_resolve(*class.super, name);
  }
  return NULL;
}

sl_imp sl_func_resolve(sl_value context, sl_symbol name) {
  sl_class_ref class = sl_get_class(context);

  if (class == NULL) {
    return NULL;
  }

  if (context.type == sl_type_class) {
    return sl_class_static_func_resolve(*class, name);
  } else {
    return sl_class_instance_func_resolve(*class, name);
  }
}

sl_imp sl_func_resolve_super(sl_value context, sl_class_ref class, sl_symbol name) {
  //  super can only be called on object or class
  assert(context.type == sl_type_object || context.type == sl_type_class);

  if (context.type == sl_type_object) {
    if (class != NULL && class->super != NULL)
      return sl_class_instance_func_resolve(*class->super, name);
  }
  if (context.type == sl_type_class) {
    if (class != NULL && class->super != NULL)
      return sl_class_static_func_resolve(*class->super, name);
  }
  return NULL;
}

sl_value sl_call(sl_value context, sl_symbol name, sl_index length, ...) {
  va_list list;
  va_start(list, length);
  sl_value result = sl_v_call(context, name, length, list);
  va_end(list);
  return result;
}

sl_value sl_v_call(sl_value context, sl_symbol name, sl_index length, va_list list) {
  sl_imp func = sl_func_resolve(context, name);
  if (func != NULL) {
    return sl_imp_v_call(func, context, length, list);
  }
  return sl_nil_value;
}

sl_value sl_call_super(sl_value context,
                       sl_class_ref class,
                       sl_symbol name,
                       sl_index length,
                       ...) {
  va_list list;
  va_start(list, length);
  sl_value result = sl_v_call_super(context, class, name, length, list);
  va_end(list);
  return result;
}

sl_value sl_v_call_super(sl_value context,
                         sl_class_ref class,
                         sl_symbol name,
                         sl_index length,
                         va_list list) {
  sl_imp func = sl_func_resolve_super(context, class, name);
  if (func != NULL) {
    return sl_imp_v_call(func, context, length, list);
  }
  return sl_nil_value;
}

sl_value sl_class_members_get(sl_class class, sl_symbol name) {
  return sl_members_get(*class.members, name);
}

sl_value sl_object_members_get(sl_object object, sl_symbol name) {
  return sl_members_get(*object.members, name);
}

sl_boolean sl_class_members_set(sl_class class, sl_symbol name, sl_value value) {
  return sl_members_set(*class.members, name, value);
}

sl_boolean sl_object_members_set(sl_object object, sl_symbol name, sl_value value) {
  return sl_members_set(*object.members, name, value);
}

sl_value sl_get(sl_value value, sl_symbol name) {
  if (value.type == sl_type_object) {
    return sl_object_members_get(*value.value.as_object, name);
  }
  if (value.type == sl_type_class) {
    return sl_class_members_get(*value.value.as_class, name);
  }
  return sl_nil_value;
}

sl_value sl_create(sl_class_ref class, sl_symbol name, sl_index length, ...) {
  va_list list;
  va_start(list, length);
  sl_value result = sl_v_create(class, name, length, list);
  va_end(list);
  return result;
}

sl_value sl_v_create(sl_class_ref class, sl_symbol name, sl_index length, va_list list) {
  if (class == &sl_class_name(Number)) {
    sl_value result = sl_integer_value(0);
    return sl_v_call(result, name, length, list);
  }
  sl_object_ref object = sl_object_create(class);
  sl_value result = sl_v_call(sl_object_value(object), name, length, list);
  //  check result, if changed, destroy original object
  if (result.type != sl_type_object || result.value.as_object != object) {
    sl_object_destroy(object);
  }
  return result;
}

sl_value sl_static_get(sl_value value, sl_symbol name) {
  if (value.type == sl_type_object) {
    value = sl_class_value(value.value.as_object->class);
  }
  return sl_get(value, name);
}

sl_boolean sl_set(sl_value target_value, sl_symbol name, sl_value value) {
  if (target_value.type == sl_type_object) {
    return sl_object_members_set(*target_value.value.as_object, name, value);
  }
  if (target_value.type == sl_type_class) {
    return sl_class_members_set(*target_value.value.as_class, name, value);
  }
  return sl_false;
}

sl_boolean sl_static_set(sl_value target_value, sl_symbol name, sl_value value) {
  if (target_value.type == sl_type_object) {
    target_value = sl_class_value(target_value.value.as_object->class);
  }
  return sl_set(target_value, name, value);
}
