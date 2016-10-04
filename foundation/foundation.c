//
// Created by Yanke Guo on 2016/9/27.
//
// TODO: should be removed after foundation library completed

#define __SYLVA_SOURCE__
#include "sylva/runtime.h"

#include <math.h>

/***********************************************************************************************************************
 * Number
 **********************************************************************************************************************/


sylva_value SYLVA_Number_I_init(sylva_value self, sylva_args arguments) {
  return self;
}

sylva_value SYLVA_Number_I_not(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 0,
               "wrong number of arguments for operator !, expecting 0, got %ld",
               arguments.length);
  return sylva_boolean_value(!sylva_to_boolean(self));
}

sylva_value SYLVA_Number_I_add(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length > 0, "wrong number of arguments for operator +, expecting >= 1, got 0");
  sylva_trans_to_numeric(&self);
  //  Iterate over arguments
  for (sylva_index i = 0; i < arguments.length; i++) {
    sylva_value arg = arguments.values[i];
    sylva_trans_to_numeric(&arg);
    switch (arg.type) {
    case sylva_type_integer: {
      //  float + integer, integer + integer
      if (self.type == sylva_type_float) {
        self.float_value += arg.integer_value;
      } else {
        self.integer_value += arg.integer_value;
      }
    }
      break;
    case sylva_type_float: {
      //  integer + float, convert self to float and continue
      sylva_trans_to_float(&self);
      //  float + float
      self.float_value += arg.float_value;
    }
      break;
    default:break;
    }
  }
  return self;
}

sylva_value SYLVA_Number_I_sub(sylva_value self, sylva_args arguments) {
  sylva_trans_to_numeric(&self);
  //  arguments count == 0, i.e prefix `-` operator
  if (arguments.length == 0) {
    switch (self.type) {
      //  integer
    case sylva_type_integer: {
      self.integer_value = -self.integer_value;
    }
      break;
      //  float
    case sylva_type_float: {
      self.float_value = -self.float_value;
    }
      break;
    default:break;
    }
    return self;
  } else {
    //  arguments count != 0, i.e infix `-` operator
    for (sylva_index i = 0; i < arguments.length; i++) {
      //  get the reverted value by recursively invoke SYLVA_Number_I_sub
      sylva_value reverted = SYLVA_Number_I_sub(arguments.values[i], sylva_args_empty);
      //  add the reverted value
      self = SYLVA_Number_I_add(self, sylva_args_make(1, reverted));
    }
    return self;
  }
}

sylva_value SYLVA_Number_I_mul(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length > 0, "wrong number of arguments for operator *, expecting >= 1, got 0");
  sylva_trans_to_numeric(&self);
  for (sylva_index i = 0; i < arguments.length; i++) {
    sylva_value arg = arguments.values[i];
    sylva_trans_to_numeric(&arg);
    switch (arg.type) {
    case sylva_type_integer: {
      //  float * integer
      if (self.type == sylva_type_float) {
        self.float_value *= arg.integer_value;
      }
      // integer * integer
      if (self.type == sylva_type_integer) {
        self.integer_value *= arg.integer_value;
      }
    }
      break;
    case sylva_type_float: {
      //  integer * float
      sylva_trans_to_float(&self);
      //  float * float
      if (self.type == sylva_type_float) {
        self.float_value *= arg.float_value;
      }
    }
      break;
    default:break;
    }
  }
  return self;
}

sylva_value SYLVA_Number_I_div(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length > 0, "wrong number of arguments for operator /, expecting >= 1, got 0");
  sylva_trans_to_numeric(&self);
  for (sylva_index i = 0; i < arguments.length; i++) {
    sylva_value arg = arguments.values[i];
    sylva_trans_to_numeric(&arg);
    switch (arg.type) {
    case sylva_type_integer: {
      //  float * integer
      if (self.type == sylva_type_float) {
        self.float_value /= arg.integer_value;
      }
      // integer * integer
      if (self.type == sylva_type_integer) {
        self.integer_value /= arg.integer_value;
      }
    }
      break;
    case sylva_type_float: {
      //  integer * float
      sylva_trans_to_float(&self);
      //  float * float
      if (self.type == sylva_type_float) {
        self.float_value /= arg.float_value;
      }
    }
      break;
    default:break;
    }
  }
  return self;
}

sylva_value SYLVA_Number_I_mod(sylva_value self, sylva_args arguments) {
  sylva_trans_to_numeric(&self);
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator %%, expecting 1, got %ld",
               arguments.length);
  sylva_value value = arguments.values[0];
  sylva_trans_to_numeric(&value);
  if (value.type == sylva_type_integer) {
    if (self.type == sylva_type_float) {
      return sylva_float_value(fmod(self.float_value, (sylva_float) value.integer_value));
    } else {
      return sylva_integer_value(self.integer_value % value.integer_value);
    }
  } else {
    if (self.type == sylva_type_float) {
      return sylva_float_value(fmod(self.float_value, value.float_value));
    } else {
      return sylva_integer_value(self.integer_value % (sylva_integer) value.float_value);
    }
  }
}

sylva_value SYLVA_Number_I_compare(sylva_value self, sylva_args arguments) {
  sylva_trans_to_numeric(&self);
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator <>, expecting 1, got %ld",
               arguments.length);
  sylva_value value = arguments.values[0];
  sylva_trans_to_numeric(&value);
  if (value.type == sylva_type_integer) {
    if (self.type == sylva_type_float) {
      return sylva_integer_value(sylva_compare(self.float_value, value.integer_value));
    } else {
      return sylva_integer_value(sylva_compare(self.integer_value, value.integer_value));
    }
  } else {
    if (self.type == sylva_type_float) {
      return sylva_integer_value(sylva_compare(self.float_value, value.float_value));
    } else {
      return sylva_integer_value(sylva_compare(self.integer_value, value.float_value));
    }
  }
}

sylva_value SYLVA_Number_I_lt(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator <, expecting 1, got %ld",
               arguments.length);
  sylva_value result = SYLVA_Number_I_compare(self, arguments);
  return sylva_boolean_value(result.integer_value == sylva_descending);
}

sylva_value SYLVA_Number_I_lt_eq(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator <=, expecting 1, got %ld",
               arguments.length);
  sylva_value result = SYLVA_Number_I_compare(self, arguments);
  return sylva_boolean_value(result.integer_value == sylva_descending || result.integer_value == sylva_same);
}

sylva_value SYLVA_Number_I_gt(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator >, expecting 1, got %ld",
               arguments.length);
  sylva_value result = SYLVA_Number_I_compare(self, arguments);
  return sylva_boolean_value(result.integer_value == sylva_ascending);
}

sylva_value SYLVA_Number_I_gt_eq(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator >=, expecting 1, got %ld",
               arguments.length);
  sylva_value result = SYLVA_Number_I_compare(self, arguments);
  return sylva_boolean_value(result.integer_value == sylva_ascending || result.integer_value == sylva_same);
}

sylva_value SYLVA_Number_I_eq(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator ==, expecting 1, got %ld",
               arguments.length);
  sylva_value result = SYLVA_Number_I_compare(self, arguments);
  return sylva_boolean_value(result.integer_value == sylva_same);
}

sylva_value SYLVA_Number_I_not_eq(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator !=, expecting 1, got %ld",
               arguments.length);
  sylva_value result = SYLVA_Number_I_compare(self, arguments);
  return sylva_boolean_value(result.integer_value != sylva_same);
}

sylva_value SYLVA_Number_I_or(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator ||, expecting 1, got %ld",
               arguments.length);
  sylva_value value = arguments.values[0];
  return sylva_boolean_value(sylva_to_boolean(self) || sylva_to_boolean(value));
}

sylva_value SYLVA_Number_I_and(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator &&, expecting 1, got %ld",
               arguments.length);
  sylva_value value = arguments.values[0];
  return sylva_boolean_value(sylva_to_boolean(self) && sylva_to_boolean(value));
}

sylva_value SYLVA_Number_I_bit_or(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator |, expecting 1, got %ld",
               arguments.length);
  sylva_value value = arguments.values[0];
  sylva_trans_to_integer(&self);
  sylva_trans_to_integer(&value);
  return sylva_integer_value(self.integer_value | value.integer_value);
}

sylva_value SYLVA_Number_I_bit_and(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator &, expecting 1, got %ld",
               arguments.length);
  sylva_value value = arguments.values[0];
  sylva_trans_to_integer(&self);
  sylva_trans_to_integer(&value);
  return sylva_integer_value(self.integer_value & value.integer_value);
}

sylva_value SYLVA_Number_I_bit_xor(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator ^, expecting 1, got %ld",
               arguments.length);
  sylva_value value = arguments.values[0];
  sylva_trans_to_integer(&self);
  sylva_trans_to_integer(&value);
  return sylva_integer_value(self.integer_value ^ value.integer_value);
}

sylva_value SYLVA_Number_I_rshift(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator >>, expecting 1, got %ld",
               arguments.length);
  sylva_value value = arguments.values[0];
  sylva_trans_to_integer(&self);
  sylva_trans_to_integer(&value);
  return sylva_integer_value(self.integer_value >> value.integer_value);
}

sylva_value SYLVA_Number_I_lshift(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 1,
               "wrong number of arguments for operator <<, expecting 1, got %ld",
               arguments.length);
  sylva_value value = arguments.values[0];
  sylva_trans_to_integer(&self);
  sylva_trans_to_integer(&value);
  return sylva_integer_value(self.integer_value << value.integer_value);
}

sylva_value SYLVA_Number_I_abs(sylva_value self, sylva_args arguments) {
  if (self.type == sylva_type_float) {
    return sylva_float_value(fabs(self.float_value));
  } else if (self.type == sylva_type_integer) {
    return sylva_integer_value(labs(self.integer_value));
  } else {
    return self;
  }
}

sylva_value SYLVA_Number_I_to_i(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 0,
               "wrong number of arguments for function 'to_i', expecting 0, got %ld",
               arguments.length);
  sylva_trans_to_integer(&self);
  return self;
}

sylva_value SYLVA_Number_I_to_f(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 0,
               "wrong number of arguments for function 'to_f', expecting 0, got %ld",
               arguments.length);
  sylva_trans_to_float(&self);
  return self;
}

sylva_value SYLVA_Number_I_to_b(sylva_value self, sylva_args arguments) {
  sylva_assert(arguments.length == 0,
               "wrong number of arguments for function 'to_b', expecting 0, got %ld",
               arguments.length);
  sylva_trans_to_boolean(&self);
  return self;
}

/***********************************************************************************************************************
 * Object
 **********************************************************************************************************************/

#define assert_is_object(V) assert((V).type == sylva_type_object)

sylva_value SYLVA_Object_I_init(sylva_value self, sylva_args arguments) {
  return self;
}

sylva_value SYLVA_Object_I_class(sylva_value self, sylva_args arguments) {
  assert_is_object(self);
  return sylva_class_value(self.object_value->class);
}

/***********************************************************************************************************************
 * Class Declarations
 **********************************************************************************************************************/

SYLVA_WEAK sylva_class SYLVA_Number = {
    .name = "Number",
    .super = NULL,
    .static_funcs = NULL,
    .instance_funcs = &sylva_funcs_make(26,
                                        sylva_func_item("init", &SYLVA_Number_I_init),
                                        sylva_func_item("not", &SYLVA_Number_I_not),
                                        sylva_func_item("add", &SYLVA_Number_I_add),
                                        sylva_func_item("sub", &SYLVA_Number_I_sub),
                                        sylva_func_item("mul", &SYLVA_Number_I_mul),
                                        sylva_func_item("div", &SYLVA_Number_I_div),
                                        sylva_func_item("mod", &SYLVA_Number_I_mod),
                                        sylva_func_item("compare", &SYLVA_Number_I_compare),
                                        sylva_func_item("lt", &SYLVA_Number_I_lt),
                                        sylva_func_item("lt_eq", &SYLVA_Number_I_lt_eq),
                                        sylva_func_item("gt", &SYLVA_Number_I_gt),
                                        sylva_func_item("gt_eq", &SYLVA_Number_I_gt_eq),
                                        sylva_func_item("eq", &SYLVA_Number_I_eq),
                                        sylva_func_item("not_eq", &SYLVA_Number_I_not_eq),
                                        sylva_func_item("or", &SYLVA_Number_I_or),
                                        sylva_func_item("and", &SYLVA_Number_I_and),
                                        sylva_func_item("or", &SYLVA_Number_I_or),
                                        sylva_func_item("bit_or", &SYLVA_Number_I_bit_or),
                                        sylva_func_item("bit_and", &SYLVA_Number_I_bit_and),
                                        sylva_func_item("bit_xor", &SYLVA_Number_I_bit_xor),
                                        sylva_func_item("rshift", &SYLVA_Number_I_rshift),
                                        sylva_func_item("lshift", &SYLVA_Number_I_lshift),
                                        sylva_func_item("to_i", &SYLVA_Number_I_to_i),
                                        sylva_func_item("to_f", &SYLVA_Number_I_to_f),
                                        sylva_func_item("to_b", &SYLVA_Number_I_to_b),
                                        sylva_func_item("abs", &SYLVA_Number_I_abs),
    ),
    .static_member_defs = NULL,
    .instance_member_defs = NULL,
    .members = NULL,
    .deinitializor = NULL,
};

SYLVA_WEAK sylva_class SYLVA_Object = {
    .name = "Object",
    .super = NULL,
    .static_funcs = NULL,
    .instance_funcs = &sylva_funcs_make(3,
                                        sylva_func_item("init", &SYLVA_Object_I_init),
                                        sylva_func_item("class", &SYLVA_Object_I_class),
    ),
    .static_member_defs = NULL,
    .instance_member_defs = NULL,
    .members = NULL,
    .deinitializor = NULL,
};
