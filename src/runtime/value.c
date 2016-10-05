//
// Created by Ryan on 2016/9/26.
//

#define __SYLVA_RUNTIME_SOURCE__
#include "sylva/runtime.h"

void sl_retain(sl_value_ref value) {
  if (value->type == sl_type_object) {
    sl_object_retain(value->value.as_object);
  }
}

void sl_release(sl_value_ref value) {
  if (value->type == sl_type_object) {
    sl_object_ref result = sl_object_release(value->value.as_object);
    //  set the sl_value to nil if object destroyed
    if (result == NULL) {
      value->type = sl_type_nil;
      value->value.as_integer = 0;
    }
  }
}

sl_boolean sl_to_boolean(sl_value value) {
  switch (value.type) {
    //  pointer
  case sl_type_object:
  case sl_type_class:
  case sl_type_pointer: return value.value.as_pointer != NULL;
    //  primitive values
  case sl_type_integer:return value.value.as_integer != 0;
  case sl_type_float: return value.value.as_float != 0;
  case sl_type_nil: return sl_false;
  case sl_type_boolean:return value.value.as_boolean;
  default:return sl_false;
  }
}

sl_integer sl_to_integer(sl_value value) {
  switch (value.type) {
  case sl_type_integer:return value.value.as_integer;
  case sl_type_float: return (sl_integer) value.value.as_float;
  case sl_type_nil: return 0;
  case sl_type_boolean:return value.value.as_boolean ? 1 : 0;
  default:return 0;
  }
}

sl_float sl_to_float(sl_value value) {
  switch (value.type) {
  case sl_type_integer:return (sl_float) value.value.as_integer;
  case sl_type_float: return value.value.as_float;
  case sl_type_nil: return 0;
  case sl_type_boolean:return value.value.as_boolean ? 1 : 0;
  default:return 0;
  }
}

void sl_trans_to_numeric(sl_value_ref value) {
  switch (value->type) {
  case sl_type_nil: {
    value->type = sl_type_integer;
    value->value.as_integer = 0;
    break;
  }
  case sl_type_boolean: {
    value->type = sl_type_integer;
    value->value.as_integer = value->value.as_boolean ? 1 : 0;
    break;
  }
  case sl_type_integer:
  case sl_type_float: {
    break;
  }
  default: {
    value->type = sl_type_integer;
    value->value.as_integer = 0;
  }
  }
}

void sl_trans_to_float(sl_value_ref value) {
  value->value.as_float = sl_to_float(*value);
  value->type = sl_type_float;
}

void sl_trans_to_integer(sl_value_ref value) {
  value->value.as_integer = sl_to_integer(*value);
  value->type = sl_type_integer;
}

void sl_trans_to_boolean(sl_value_ref value) {
  value->value.as_boolean = sl_to_boolean(*value);
  value->type = sl_type_boolean;
}
