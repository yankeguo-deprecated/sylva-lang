//
// Created by Yanke Guo on 2016/9/27.
//
// TODO: should be removed after foundation library completed

#define __SYLVA_RUNTIME_SOURCE__
#include "sylva/runtime.h"

#include <math.h>

/***********************************************************************************************************************
 * Number
 **********************************************************************************************************************/


sl_value SYLVA_Number_I_init(sl_value self, sl_args arguments) {
  return self;
}

sl_value SYLVA_Number_I_not(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 0,
            "wrong number of arguments for operator !, expecting 0, got %ld",
            arguments.length);
  return sl_boolean_value(!sl_to_boolean(self));
}

sl_value SYLVA_Number_I_add(sl_value self, sl_args arguments) {
  sl_assert(arguments.length > 0, "wrong number of arguments for operator +, expecting >= 1, got 0");
  sl_trans_to_numeric(&self);
  //  Iterate over arguments
  for (sl_index i = 0; i < arguments.length; i++) {
    sl_value arg = arguments.values[i];
    sl_trans_to_numeric(&arg);
    switch (arg.type) {
    case sl_type_integer: {
      //  float + integer, integer + integer
      if (self.type == sl_type_float) {
        self.float_value += arg.integer_value;
      } else {
        self.integer_value += arg.integer_value;
      }
    }
      break;
    case sl_type_float: {
      //  integer + float, convert self to float and continue
      sl_trans_to_float(&self);
      //  float + float
      self.float_value += arg.float_value;
    }
      break;
    default:break;
    }
  }
  return self;
}

sl_value SYLVA_Number_I_sub(sl_value self, sl_args arguments) {
  sl_trans_to_numeric(&self);
  //  arguments count == 0, i.e prefix `-` operator
  if (arguments.length == 0) {
    switch (self.type) {
      //  integer
    case sl_type_integer: {
      self.integer_value = -self.integer_value;
    }
      break;
      //  float
    case sl_type_float: {
      self.float_value = -self.float_value;
    }
      break;
    default:break;
    }
    return self;
  } else {
    //  arguments count != 0, i.e infix `-` operator
    for (sl_index i = 0; i < arguments.length; i++) {
      //  get the reverted value by recursively invoke SYLVA_Number_I_sub
      sl_value reverted = SYLVA_Number_I_sub(arguments.values[i], sl_args_empty);
      //  add the reverted value
      self = SYLVA_Number_I_add(self, sl_args_make(1, reverted));
    }
    return self;
  }
}

sl_value SYLVA_Number_I_mul(sl_value self, sl_args arguments) {
  sl_assert(arguments.length > 0, "wrong number of arguments for operator *, expecting >= 1, got 0");
  sl_trans_to_numeric(&self);
  for (sl_index i = 0; i < arguments.length; i++) {
    sl_value arg = arguments.values[i];
    sl_trans_to_numeric(&arg);
    switch (arg.type) {
    case sl_type_integer: {
      //  float * integer
      if (self.type == sl_type_float) {
        self.float_value *= arg.integer_value;
      }
      // integer * integer
      if (self.type == sl_type_integer) {
        self.integer_value *= arg.integer_value;
      }
    }
      break;
    case sl_type_float: {
      //  integer * float
      sl_trans_to_float(&self);
      //  float * float
      if (self.type == sl_type_float) {
        self.float_value *= arg.float_value;
      }
    }
      break;
    default:break;
    }
  }
  return self;
}

sl_value SYLVA_Number_I_div(sl_value self, sl_args arguments) {
  sl_assert(arguments.length > 0, "wrong number of arguments for operator /, expecting >= 1, got 0");
  sl_trans_to_numeric(&self);
  for (sl_index i = 0; i < arguments.length; i++) {
    sl_value arg = arguments.values[i];
    sl_trans_to_numeric(&arg);
    switch (arg.type) {
    case sl_type_integer: {
      //  float * integer
      if (self.type == sl_type_float) {
        self.float_value /= arg.integer_value;
      }
      // integer * integer
      if (self.type == sl_type_integer) {
        self.integer_value /= arg.integer_value;
      }
    }
      break;
    case sl_type_float: {
      //  integer * float
      sl_trans_to_float(&self);
      //  float * float
      if (self.type == sl_type_float) {
        self.float_value /= arg.float_value;
      }
    }
      break;
    default:break;
    }
  }
  return self;
}

sl_value SYLVA_Number_I_mod(sl_value self, sl_args arguments) {
  sl_trans_to_numeric(&self);
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator %%, expecting 1, got %ld",
            arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_numeric(&value);
  if (value.type == sl_type_integer) {
    if (self.type == sl_type_float) {
      return sl_float_value(fmod(self.float_value, (sl_float) value.integer_value));
    } else {
      return sl_integer_value(self.integer_value % value.integer_value);
    }
  } else {
    if (self.type == sl_type_float) {
      return sl_float_value(fmod(self.float_value, value.float_value));
    } else {
      return sl_integer_value(self.integer_value % (sl_integer) value.float_value);
    }
  }
}

sl_value SYLVA_Number_I_compare(sl_value self, sl_args arguments) {
  sl_trans_to_numeric(&self);
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator <>, expecting 1, got %ld",
            arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_numeric(&value);
  if (value.type == sl_type_integer) {
    if (self.type == sl_type_float) {
      return sl_integer_value(sl_compare(self.float_value, value.integer_value));
    } else {
      return sl_integer_value(sl_compare(self.integer_value, value.integer_value));
    }
  } else {
    if (self.type == sl_type_float) {
      return sl_integer_value(sl_compare(self.float_value, value.float_value));
    } else {
      return sl_integer_value(sl_compare(self.integer_value, value.float_value));
    }
  }
}

sl_value SYLVA_Number_I_lt(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator <, expecting 1, got %ld",
            arguments.length);
  sl_value result = SYLVA_Number_I_compare(self, arguments);
  return sl_boolean_value(result.integer_value == sl_descending);
}

sl_value SYLVA_Number_I_lt_eq(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator <=, expecting 1, got %ld",
            arguments.length);
  sl_value result = SYLVA_Number_I_compare(self, arguments);
  return sl_boolean_value(result.integer_value == sl_descending || result.integer_value == sl_same);
}

sl_value SYLVA_Number_I_gt(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator >, expecting 1, got %ld",
            arguments.length);
  sl_value result = SYLVA_Number_I_compare(self, arguments);
  return sl_boolean_value(result.integer_value == sl_ascending);
}

sl_value SYLVA_Number_I_gt_eq(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator >=, expecting 1, got %ld",
            arguments.length);
  sl_value result = SYLVA_Number_I_compare(self, arguments);
  return sl_boolean_value(result.integer_value == sl_ascending || result.integer_value == sl_same);
}

sl_value SYLVA_Number_I_eq(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator ==, expecting 1, got %ld",
            arguments.length);
  sl_value result = SYLVA_Number_I_compare(self, arguments);
  return sl_boolean_value(result.integer_value == sl_same);
}

sl_value SYLVA_Number_I_not_eq(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator !=, expecting 1, got %ld",
            arguments.length);
  sl_value result = SYLVA_Number_I_compare(self, arguments);
  return sl_boolean_value(result.integer_value != sl_same);
}

sl_value SYLVA_Number_I_or(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator ||, expecting 1, got %ld",
            arguments.length);
  sl_value value = arguments.values[0];
  return sl_boolean_value(sl_to_boolean(self) || sl_to_boolean(value));
}

sl_value SYLVA_Number_I_and(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator &&, expecting 1, got %ld",
            arguments.length);
  sl_value value = arguments.values[0];
  return sl_boolean_value(sl_to_boolean(self) && sl_to_boolean(value));
}

sl_value SYLVA_Number_I_bit_or(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator |, expecting 1, got %ld",
            arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_integer(&self);
  sl_trans_to_integer(&value);
  return sl_integer_value(self.integer_value | value.integer_value);
}

sl_value SYLVA_Number_I_bit_and(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator &, expecting 1, got %ld",
            arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_integer(&self);
  sl_trans_to_integer(&value);
  return sl_integer_value(self.integer_value & value.integer_value);
}

sl_value SYLVA_Number_I_bit_xor(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator ^, expecting 1, got %ld",
            arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_integer(&self);
  sl_trans_to_integer(&value);
  return sl_integer_value(self.integer_value ^ value.integer_value);
}

sl_value SYLVA_Number_I_rshift(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator >>, expecting 1, got %ld",
            arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_integer(&self);
  sl_trans_to_integer(&value);
  return sl_integer_value(self.integer_value >> value.integer_value);
}

sl_value SYLVA_Number_I_lshift(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator <<, expecting 1, got %ld",
            arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_integer(&self);
  sl_trans_to_integer(&value);
  return sl_integer_value(self.integer_value << value.integer_value);
}

sl_value SYLVA_Number_I_abs(sl_value self, sl_args arguments) {
  if (self.type == sl_type_float) {
    return sl_float_value(fabs(self.float_value));
  } else if (self.type == sl_type_integer) {
    return sl_integer_value(labs(self.integer_value));
  } else {
    return self;
  }
}

sl_value SYLVA_Number_I_to_i(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 0,
            "wrong number of arguments for function 'to_i', expecting 0, got %ld",
            arguments.length);
  sl_trans_to_integer(&self);
  return self;
}

sl_value SYLVA_Number_I_to_f(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 0,
            "wrong number of arguments for function 'to_f', expecting 0, got %ld",
            arguments.length);
  sl_trans_to_float(&self);
  return self;
}

sl_value SYLVA_Number_I_to_b(sl_value self, sl_args arguments) {
  sl_assert(arguments.length == 0,
            "wrong number of arguments for function 'to_b', expecting 0, got %ld",
            arguments.length);
  sl_trans_to_boolean(&self);
  return self;
}

/***********************************************************************************************************************
 * Object
 **********************************************************************************************************************/

#define assert_is_object(V) assert((V).type == sl_type_object)

sl_value SYLVA_Object_I_init(sl_value self, sl_args arguments) {
  return self;
}

sl_value SYLVA_Object_I_class(sl_value self, sl_args arguments) {
  assert_is_object(self);
  return sl_class_value(self.object_value->class);
}

/***********************************************************************************************************************
 * Class Declarations
 **********************************************************************************************************************/

sl_class SYLVA_Number = {
    .name = "Number",
    .super = NULL,
    .static_funcs = NULL,
    .instance_funcs = &sl_funcs_make(26,
                                     sl_func_item("init", &SYLVA_Number_I_init),
                                     sl_func_item("not", &SYLVA_Number_I_not),
                                     sl_func_item("add", &SYLVA_Number_I_add),
                                     sl_func_item("sub", &SYLVA_Number_I_sub),
                                     sl_func_item("mul", &SYLVA_Number_I_mul),
                                     sl_func_item("div", &SYLVA_Number_I_div),
                                     sl_func_item("mod", &SYLVA_Number_I_mod),
                                     sl_func_item("compare", &SYLVA_Number_I_compare),
                                     sl_func_item("lt", &SYLVA_Number_I_lt),
                                     sl_func_item("lt_eq", &SYLVA_Number_I_lt_eq),
                                     sl_func_item("gt", &SYLVA_Number_I_gt),
                                     sl_func_item("gt_eq", &SYLVA_Number_I_gt_eq),
                                     sl_func_item("eq", &SYLVA_Number_I_eq),
                                     sl_func_item("not_eq", &SYLVA_Number_I_not_eq),
                                     sl_func_item("or", &SYLVA_Number_I_or),
                                     sl_func_item("and", &SYLVA_Number_I_and),
                                     sl_func_item("or", &SYLVA_Number_I_or),
                                     sl_func_item("bit_or", &SYLVA_Number_I_bit_or),
                                     sl_func_item("bit_and", &SYLVA_Number_I_bit_and),
                                     sl_func_item("bit_xor", &SYLVA_Number_I_bit_xor),
                                     sl_func_item("rshift", &SYLVA_Number_I_rshift),
                                     sl_func_item("lshift", &SYLVA_Number_I_lshift),
                                     sl_func_item("to_i", &SYLVA_Number_I_to_i),
                                     sl_func_item("to_f", &SYLVA_Number_I_to_f),
                                     sl_func_item("to_b", &SYLVA_Number_I_to_b),
                                     sl_func_item("abs", &SYLVA_Number_I_abs),
    ),
    .static_member_defs = NULL,
    .instance_member_defs = NULL,
    .members = NULL,
    .deinitializor = NULL,
};

sl_class SYLVA_Object = {
    .name = "Object",
    .super = NULL,
    .static_funcs = NULL,
    .instance_funcs = &sl_funcs_make(3,
                                     sl_func_item("init", &SYLVA_Object_I_init),
                                     sl_func_item("class", &SYLVA_Object_I_class),
    ),
    .static_member_defs = NULL,
    .instance_member_defs = NULL,
    .members = NULL,
    .deinitializor = NULL,
};
