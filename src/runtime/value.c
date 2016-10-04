//
// Created by Ryan on 2016/9/26.
//

#define __SYLVA_RUNTIME_SOURCE__
#include "sylva/runtime.h"

void sl_retain(sl_value_ref value) {
  if (value->type == sl_type_object) {
    sl_object_retain(value->object_value);
  }
}

void sl_release(sl_value_ref value) {
  if (value->type == sl_type_object) {
    sl_object_ref result = sl_object_release(value->object_value);
    //  set the sl_value to nil if object destroyed
    if (result == NULL) {
      value->type = sl_type_nil;
      value->integer_value = 0;
    }
  }
}

sl_boolean sl_to_boolean(sl_value value) {
  switch (value.type) {
    //  pointer
  case sl_type_object:
  case sl_type_class:
  case sl_type_pointer: return value.pointer_value != NULL;
    //  primitive values
  case sl_type_integer:return value.integer_value != 0;
  case sl_type_float: return value.float_value != 0;
  case sl_type_nil: return sl_false;
  case sl_type_boolean:return value.boolean_value;
  default:return sl_false;
  }
}

sl_integer sl_to_integer(sl_value value) {
  switch (value.type) {
  case sl_type_integer:return value.integer_value;
  case sl_type_float: return (sl_integer) value.float_value;
  case sl_type_nil: return 0;
  case sl_type_boolean:return value.boolean_value ? 1 : 0;
  default:return 0;
  }
}

sl_float sl_to_float(sl_value value) {
  switch (value.type) {
  case sl_type_integer:return (sl_float) value.integer_value;
  case sl_type_float: return value.float_value;
  case sl_type_nil: return 0;
  case sl_type_boolean:return value.boolean_value ? 1 : 0;
  default:return 0;
  }
}

void sl_trans_to_numeric(sl_value_ref value) {
  switch (value->type) {
  case sl_type_nil: {
    value->type = sl_type_integer;
    value->integer_value = 0;
    break;
  }
  case sl_type_boolean: {
    value->type = sl_type_integer;
    value->integer_value = value->boolean_value ? 1 : 0;
    break;
  }
  case sl_type_integer:
  case sl_type_float: {
    break;
  }
  default: {
    value->type = sl_type_integer;
    value->integer_value = 0;
  }
  }
}

void sl_trans_to_float(sl_value_ref value) {
  value->float_value = sl_to_float(*value);
  value->type = sl_type_float;
}

void sl_trans_to_integer(sl_value_ref value) {
  value->integer_value = sl_to_integer(*value);
  value->type = sl_type_integer;
}

void sl_trans_to_boolean(sl_value_ref value) {
  value->boolean_value = sl_to_boolean(*value);
  value->type = sl_type_boolean;
}
