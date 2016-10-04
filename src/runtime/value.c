//
// Created by Ryan on 2016/9/26.
//

#include "sylva/runtime.h"

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

void sylva_trans_to_numeric(sylva_value_ref value) {
  switch (value->type) {
  case sylva_type_nil: {
    value->type = sylva_type_integer;
    value->integer_value = 0;
    break;
  }
  case sylva_type_boolean: {
    value->type = sylva_type_integer;
    value->integer_value = value->boolean_value ? 1 : 0;
    break;
  }
  case sylva_type_integer:
  case sylva_type_float: {
    break;
  }
  default: {
    value->type = sylva_type_integer;
    value->integer_value = 0;
  }
  }
}

void sylva_trans_to_float(sylva_value_ref value) {
  value->float_value = sylva_to_float(*value);
  value->type = sylva_type_float;
}

void sylva_trans_to_integer(sylva_value_ref value) {
  value->integer_value = sylva_to_integer(*value);
  value->type = sylva_type_integer;
}

void sylva_trans_to_boolean(sylva_value_ref value) {
  value->boolean_value = sylva_to_boolean(*value);
  value->type = sylva_type_boolean;
}
