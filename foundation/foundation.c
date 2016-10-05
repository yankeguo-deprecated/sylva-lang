//
// Created by Yanke Guo on 2016/9/27.
//
// TODO: should be removed after foundation library completed

#define __SYLVA_RUNTIME_SOURCE__
#include "sylva/runtime.h"

#include <math.h>
#include <sylva/runtime.h>

/***********************************************************************************************************************
 * Number
 **********************************************************************************************************************/

sl_func_decl_class_instance(Number, init) {
  return self;
}

sl_func_decl_class_instance(Number, not) {
  sl_assert(arguments.length == 0, "wrong number of arguments for operator !, expecting 0, got %ld", arguments.length);
  return sl_boolean_value(!sl_to_boolean(self));
}

sl_func_decl_class_instance(Number, add) {
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
        self.value.as_float += arg.value.as_integer;
      } else {
        self.value.as_integer += arg.value.as_integer;
      }
    }
      break;
    case sl_type_float: {
      //  integer + float, convert self to float and continue
      sl_trans_to_float(&self);
      //  float + float
      self.value.as_float += arg.value.as_float;
    }
      break;
    default:break;
    }
  }
  return self;
}

sl_func_decl_class_instance(Number, sub) {
  sl_trans_to_numeric(&self);
  //  arguments count == 0, i.e prefix `-` operator
  if (arguments.length == 0) {
    switch (self.type) {
      //  integer
    case sl_type_integer: {
      self.value.as_integer = -self.value.as_integer;
    }
      break;
      //  float
    case sl_type_float: {
      self.value.as_float = -self.value.as_float;
    }
      break;
    default:break;
    }
    return self;
  } else {
    //  arguments count != 0, i.e infix `-` operator
    for (sl_index i = 0; i < arguments.length; i++) {
      //  get the reverted value by recursively invoke SYLVA_Number_I_sub
      sl_value reverted = sl_func_name_class_instance(Number, sub)(arguments.values[i], sl_args_empty);
      //  add the reverted value
      self = sl_func_name_class_instance(Number, add)(self, sl_args_make(1, reverted));
    }
    return self;
  }
}

sl_func_decl_class_instance(Number, mul) {
  sl_assert(arguments.length > 0, "wrong number of arguments for operator *, expecting >= 1, got 0");
  sl_trans_to_numeric(&self);
  for (sl_index i = 0; i < arguments.length; i++) {
    sl_value arg = arguments.values[i];
    sl_trans_to_numeric(&arg);
    switch (arg.type) {
    case sl_type_integer: {
      //  float * integer
      if (self.type == sl_type_float) {
        self.value.as_float *= arg.value.as_integer;
      }
      // integer * integer
      if (self.type == sl_type_integer) {
        self.value.as_integer *= arg.value.as_integer;
      }
    }
      break;
    case sl_type_float: {
      //  integer * float
      sl_trans_to_float(&self);
      //  float * float
      if (self.type == sl_type_float) {
        self.value.as_float *= arg.value.as_float;
      }
    }
      break;
    default:break;
    }
  }
  return self;
}

sl_func_decl_class_instance(Number, div) {
  sl_assert(arguments.length > 0, "wrong number of arguments for operator /, expecting >= 1, got 0");
  sl_trans_to_numeric(&self);
  for (sl_index i = 0; i < arguments.length; i++) {
    sl_value arg = arguments.values[i];
    sl_trans_to_numeric(&arg);
    switch (arg.type) {
    case sl_type_integer: {
      //  float * integer
      if (self.type == sl_type_float) {
        self.value.as_float /= arg.value.as_integer;
      }
      // integer * integer
      if (self.type == sl_type_integer) {
        self.value.as_integer /= arg.value.as_integer;
      }
    }
      break;
    case sl_type_float: {
      //  integer * float
      sl_trans_to_float(&self);
      //  float * float
      if (self.type == sl_type_float) {
        self.value.as_float /= arg.value.as_float;
      }
    }
      break;
    default:break;
    }
  }
  return self;
}

sl_func_decl_class_instance(Number, mod) {
  sl_trans_to_numeric(&self);
  sl_assert(arguments.length == 1, "wrong number of arguments for operator %%, expecting 1, got %ld", arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_numeric(&value);
  if (value.type == sl_type_integer) {
    if (self.type == sl_type_float) {
      return sl_float_value(fmod(self.value.as_float, (sl_float) value.value.as_integer));
    } else {
      return sl_integer_value(self.value.as_integer % value.value.as_integer);
    }
  } else {
    if (self.type == sl_type_float) {
      return sl_float_value(fmod(self.value.as_float, value.value.as_float));
    } else {
      return sl_integer_value(self.value.as_integer % (sl_integer) value.value.as_float);
    }
  }
}

sl_func_decl_class_instance(Number, compare) {
  sl_trans_to_numeric(&self);
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator <>, expecting 1, got %ld",
            arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_numeric(&value);
  if (value.type == sl_type_integer) {
    if (self.type == sl_type_float) {
      return sl_integer_value(sl_compare(self.value.as_float, value.value.as_integer));
    } else {
      return sl_integer_value(sl_compare(self.value.as_integer, value.value.as_integer));
    }
  } else {
    if (self.type == sl_type_float) {
      return sl_integer_value(sl_compare(self.value.as_float, value.value.as_float));
    } else {
      return sl_integer_value(sl_compare(self.value.as_integer, value.value.as_float));
    }
  }
}

sl_func_decl_class_instance(Number, lt) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator <, expecting 1, got %ld",
            arguments.length);
  sl_value result = sl_func_name_class_instance(Number, compare(self, arguments));
  return sl_boolean_value(result.value.as_integer == sl_ascending);
}

sl_func_decl_class_instance(Number, lt_eq) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator <=, expecting 1, got %ld",
            arguments.length);
  sl_value result = sl_func_name_class_instance(Number, compare(self, arguments));
  return sl_boolean_value(result.value.as_integer == sl_ascending || result.value.as_integer == sl_same);
}

sl_func_decl_class_instance(Number, gt) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator >, expecting 1, got %ld",
            arguments.length);
  sl_value result = sl_func_name_class_instance(Number, compare(self, arguments));
  return sl_boolean_value(result.value.as_integer == sl_descending);
}

sl_func_decl_class_instance(Number, gt_eq) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator >=, expecting 1, got %ld",
            arguments.length);
  sl_value result = sl_func_name_class_instance(Number, compare(self, arguments));
  return sl_boolean_value(result.value.as_integer == sl_descending || result.value.as_integer == sl_same);
}

sl_func_decl_class_instance(Number, eq) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for operator ==, expecting 1, got %ld",
            arguments.length);
  sl_value result = sl_func_name_class_instance(Number, compare(self, arguments));
  return sl_boolean_value(result.value.as_integer == sl_same);
}

sl_func_decl_class_instance(Number, not_eq) {
  sl_assert(arguments.length == 1, "wrong number of arguments for operator !=, expecting 1, got %ld", arguments.length);
  sl_value result = sl_func_name_class_instance(Number, compare(self, arguments));
  return sl_boolean_value(result.value.as_integer != sl_same);
}

sl_func_decl_class_instance(Number, or) {
  sl_assert(arguments.length == 1, "wrong number of arguments for operator ||, expecting 1, got %ld", arguments.length);
  sl_value value = arguments.values[0];
  return sl_boolean_value(sl_to_boolean(self) || sl_to_boolean(value));
}

sl_func_decl_class_instance(Number, and) {
  sl_assert(arguments.length == 1, "wrong number of arguments for operator &&, expecting 1, got %ld", arguments.length);
  sl_value value = arguments.values[0];
  return sl_boolean_value(sl_to_boolean(self) && sl_to_boolean(value));
}

sl_func_decl_class_instance(Number, bit_or) {
  sl_assert(arguments.length == 1, "wrong number of arguments for operator |, expecting 1, got %ld", arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_integer(&self);
  sl_trans_to_integer(&value);
  return sl_integer_value(self.value.as_integer | value.value.as_integer);
}

sl_func_decl_class_instance(Number, bit_and) {
  sl_assert(arguments.length == 1, "wrong number of arguments for operator &, expecting 1, got %ld", arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_integer(&self);
  sl_trans_to_integer(&value);
  return sl_integer_value(self.value.as_integer & value.value.as_integer);
}

sl_func_decl_class_instance(Number, bit_xor) {
  sl_assert(arguments.length == 1, "wrong number of arguments for operator ^, expecting 1, got %ld", arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_integer(&self);
  sl_trans_to_integer(&value);
  return sl_integer_value(self.value.as_integer ^ value.value.as_integer);
}

sl_func_decl_class_instance(Number, rshift) {
  sl_assert(arguments.length == 1, "wrong number of arguments for operator >>, expecting 1, got %ld", arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_integer(&self);
  sl_trans_to_integer(&value);
  return sl_integer_value(self.value.as_integer >> value.value.as_integer);
}

sl_func_decl_class_instance(Number, lshift) {
  sl_assert(arguments.length == 1, "wrong number of arguments for operator <<, expecting 1, got %ld", arguments.length);
  sl_value value = arguments.values[0];
  sl_trans_to_integer(&self);
  sl_trans_to_integer(&value);
  return sl_integer_value(self.value.as_integer << value.value.as_integer);
}

sl_func_decl_class_instance(Number, abs) {
  if (self.type == sl_type_float) {
    return sl_float_value(fabs(self.value.as_float));
  } else if (self.type == sl_type_integer) {
    return sl_integer_value(labs(self.value.as_integer));
  } else {
    return self;
  }
}

sl_func_decl_class_instance(Number, to_i) {
  sl_assert(arguments.length == 0,
            "wrong number of arguments for function 'to_i', expecting 0, got %ld",
            arguments.length);
  sl_trans_to_integer(&self);
  return self;
}

sl_func_decl_class_instance(Number, to_f) {
  sl_assert(arguments.length == 0,
            "wrong number of arguments for function 'to_f', expecting 0, got %ld",
            arguments.length);
  sl_trans_to_float(&self);
  return self;
}

sl_func_decl_class_instance(Number, to_b) {
  sl_assert(arguments.length == 0,
            "wrong number of arguments for function 'to_b', expecting 0, got %ld",
            arguments.length);
  sl_trans_to_boolean(&self);
  return self;
}

/***********************************************************************************************************************
 * Object
 **********************************************************************************************************************/

sl_func_decl_class_instance(Object, init) {
  return self;
}

sl_func_decl_class_instance(Object, class) {
  return sl_class_value(self.value.as_object->class);
}

sl_func_decl_class_instance(Object, eq) {
  sl_assert(arguments.length == 1,
            "wrong number of arguments for function '==', expecting 1, got %ld",
            arguments.length);
  sl_value b = sl_args_get(arguments, 0);
  return sl_boolean_value(self.value.as_pointer == b.value.as_pointer);
}

/***********************************************************************************************************************
 * Class Declarations
 **********************************************************************************************************************/

sl_class_decl_start(Number)
        sl_class_decl_super_null
        sl_class_decl_static_funcs_null
        sl_class_decl_instance_funcs_start(26)
                            sl_class_decl_instance_class_func(Number, init)
                            sl_class_decl_instance_class_func(Number, not)
                            sl_class_decl_instance_class_func(Number, add)
                            sl_class_decl_instance_class_func(Number, sub)
                            sl_class_decl_instance_class_func(Number, mul)
                            sl_class_decl_instance_class_func(Number, div)
                            sl_class_decl_instance_class_func(Number, mod)
                            sl_class_decl_instance_class_func(Number, compare)
                            sl_class_decl_instance_class_func(Number, lt)
                            sl_class_decl_instance_class_func(Number, lt_eq)
                            sl_class_decl_instance_class_func(Number, gt)
                            sl_class_decl_instance_class_func(Number, gt_eq)
                            sl_class_decl_instance_class_func(Number, eq)
                            sl_class_decl_instance_class_func(Number, not_eq)
                            sl_class_decl_instance_class_func(Number, or)
                            sl_class_decl_instance_class_func(Number, and)
                            sl_class_decl_instance_class_func(Number, bit_or)
                            sl_class_decl_instance_class_func(Number, bit_and)
                            sl_class_decl_instance_class_func(Number, bit_xor)
                            sl_class_decl_instance_class_func(Number, rshift)
                            sl_class_decl_instance_class_func(Number, lshift)
                            sl_class_decl_instance_class_func(Number, to_i)
                            sl_class_decl_instance_class_func(Number, to_f)
                            sl_class_decl_instance_class_func(Number, to_b)
                            sl_class_decl_instance_class_func(Number, abs)
        sl_class_decl_instance_funcs_end
        sl_class_decl_static_member_defs_null
        sl_class_decl_instance_member_defs_null
        sl_class_decl_members_null
sl_class_decl_end

sl_class_decl_start(Object)
        sl_class_decl_super_null
        sl_class_decl_static_funcs_null
        sl_class_decl_instance_funcs_start(3)
                            sl_class_decl_instance_class_func(Object, init)
                            sl_class_decl_instance_class_func(Object, class)
                            sl_class_decl_instance_class_func(Object, eq)
        sl_class_decl_instance_funcs_end
        sl_class_decl_static_member_defs_null
        sl_class_decl_instance_member_defs_null
        sl_class_decl_members_null
sl_class_decl_end
