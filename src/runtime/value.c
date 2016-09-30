//
// Created by Ryan on 2016/9/26.
//

#include "sylva/runtime.h"

#include <stdlib.h>
#include <sylva/runtime.h>
#include <sylva/foundation.h>

sylva_value sylva_create(sylva_class_ref class, sylva_func_id func_id, sylva_index length, ...) {
  va_list list;
  va_start(list, length);
  sylva_value result = sylva_v_create(class, func_id, length, list);
  va_end(list);
  return result;
}

sylva_value sylva_v_create(sylva_class_ref class, sylva_func_id func_id, sylva_index length, va_list list) {
  if (class == &SYLVA_Number) {
    sylva_value result = SYLVA_Number_S_new(sylva_class_value(&SYLVA_Number), sylva_args_empty);
    return sylva_v_call(result, func_id, length, list);
  }
  sylva_object_ref object = sylva_object_create(class);
  sylva_value result = sylva_v_call(sylva_object_value(object), func_id, length, list);
  //  check result, if changed, destroy orignal object
  if (result.type != sylva_type_object || result.object_value != object) {
    sylva_object_destroy(object);
  }
  return result;
}

void sylva_retain(sylva_value_ref value) {
  if (value->type == sylva_type_object) {
    sylva_object_retain(value->object_value);
  }
}

void sylva_release(sylva_value_ref value) {
  if (value->type == sylva_type_object) {
    sylva_object_ref result = sylva_object_release(value->object_value);
    //  set the sylva_value to nil if object destroyed
    if (result == NULL) {
      value->type = sylva_type_nil;
      value->integer_value = 0;
    }
  }
}

sylva_boolean sylva_to_boolean(sylva_value value) {
  switch (value.type) {
    //  pointer
  case sylva_type_object:
  case sylva_type_class:
  case sylva_type_pointer: return value.pointer_value != NULL;
    //  primitive values
  case sylva_type_integer:return value.integer_value != 0;
  case sylva_type_float: return value.float_value != 0;
  case sylva_type_nil: return sylva_false;
  case sylva_type_boolean:return value.boolean_value;
  default:return sylva_false;
  }
}

sylva_integer sylva_to_integer(sylva_value value) {
  switch (value.type) {
  case sylva_type_integer:return value.integer_value;
  case sylva_type_float: return (sylva_integer) value.float_value;
  case sylva_type_nil: return 0;
  case sylva_type_boolean:return value.boolean_value ? 1 : 0;
  default:return 0;
  }
}

sylva_float sylva_to_float(sylva_value value) {
  switch (value.type) {
  case sylva_type_integer:return (sylva_float) value.integer_value;
  case sylva_type_float: return value.float_value;
  case sylva_type_nil: return 0;
  case sylva_type_boolean:return value.boolean_value ? 1 : 0;
  default:return 0;
  }
}
