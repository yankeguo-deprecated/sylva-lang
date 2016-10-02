//
// Created by Yanke Guo on 2016/9/27.
//

#define __SYLVA_SOURCE__
#include "sylva/runtime.h"
#include "sylva/foundation.h"

#include <assert.h>
#include <sylva/runtime.h>
#include <stdlib.h>
#include <math.h>

/***********************************************************************************************************************
 * Internal Macros
 **********************************************************************************************************************/

/**
 * assert a value is a primitive value
 */
#define assert_primitive(V) assert((V).type & sylva_type_primitive)

/**
 * sanitize nil, boolean type to integer
 */
#define sanitize_primitive(V)\
  if ((V).type == sylva_type_boolean) {\
    (V).type = sylva_type_integer;\
    (V).integer_value = (V).boolean_value ? 1 : 0;\
  }\
  if ((V).type == sylva_type_nil) {\
    (V).type = sylva_type_integer;\
    (V).integer_value = 0;\
  }

#define integer_to_float(V)\
  if ((V).type == sylva_type_integer) {\
    (V).type = sylva_type_float;\
    (V).float_value = (sylva_float)(V).integer_value;\
  }

#define float_to_integer(V)\
  if ((V).type == sylva_type_float) {\
    (V).type = sylva_type_integer;\
    (V).integer_value = (sylva_integer)(V).float_value;\
  }

/**
 * convert nil, boolean, float to integer
 */
#define to_integer(V) sanitize_primitive(V); float_to_integer(V);

/**
 * compare two values and returns sylva_compare_result
 */
#define to_compare(A, B) ((A) == (B) ? sylva_same : ((A) > (B) ? sylva_descending : sylva_ascending))

/***********************************************************************************************************************
 * Number
 **********************************************************************************************************************/

sylva_class SYLVA_Number = {
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

sylva_value SYLVA_Number_I_init(sylva_value context, sylva_args args) {
  return context;
}

sylva_value SYLVA_Number_I_not(sylva_value context, sylva_args args) {
  assert_primitive(context);
  return sylva_boolean_value(!sylva_to_boolean(context));
}

sylva_value SYLVA_Number_I_add(sylva_value context, sylva_args args) {
  assert_primitive(context);
  sanitize_primitive(context);
  //  Iterate over arguments
  for (sylva_index i = 0; i < args.length; i++) {
    sylva_value arg = args.values[i];
    assert_primitive(arg);
    sanitize_primitive(arg);
    switch (arg.type) {
    case sylva_type_integer: {
      //  float + integer, integer + integer
      if (context.type == sylva_type_float) {
        context.float_value += arg.integer_value;
      } else {
        context.integer_value += arg.integer_value;
      }
    }
      break;
    case sylva_type_float: {
      //  integer + float, convert context to float and continue
      integer_to_float(context);
      //  float + float
      context.float_value += arg.float_value;
    }
      break;
    default:break;
    }
  }
  return context;
}

sylva_value SYLVA_Number_I_sub(sylva_value context, sylva_args args) {
  assert_primitive(context);
  sanitize_primitive(context);
  //  arguments count == 0, i.e prefix `-` operator
  if (args.length == 0) {
    switch (context.type) {
      //  integer
    case sylva_type_integer: {
      context.integer_value = -context.integer_value;
    }
      break;
      //  float
    case sylva_type_float: {
      context.float_value = -context.float_value;
    }
      break;
    default:break;
    }
    return context;
  } else {
    //  arguments count != 0, i.e infix `-` operator
    for (sylva_index i = 0; i < args.length; i++) {
      //  get the reverted value by recursively invoke SYLVA_Number_I_sub
      sylva_value reverted = SYLVA_Number_I_sub(args.values[i], sylva_args_empty);
      //  add the reverted value
      context = SYLVA_Number_I_add(context, sylva_args_make(1, reverted));
    }
    return context;
  }
}

sylva_value SYLVA_Number_I_mul(sylva_value context, sylva_args args) {
  assert_primitive(context);
  sanitize_primitive(context);
  for (sylva_index i = 0; i < args.length; i++) {
    sylva_value arg = args.values[i];
    assert_primitive(arg);
    sanitize_primitive(arg);
    switch (arg.type) {
    case sylva_type_integer: {
      //  float * integer
      if (context.type == sylva_type_float) {
        context.float_value *= arg.integer_value;
      }
      // integer * integer
      if (context.type == sylva_type_integer) {
        context.integer_value *= arg.integer_value;
      }
    }
      break;
    case sylva_type_float: {
      //  integer * float
      integer_to_float(context);
      //  float * float
      if (context.type == sylva_type_float) {
        context.float_value *= arg.float_value;
      }
    }
      break;
    default:break;
    }
  }
  return context;
}

sylva_value SYLVA_Number_I_div(sylva_value context, sylva_args args) {
  assert_primitive(context);
  sanitize_primitive(context);
  for (sylva_index i = 0; i < args.length; i++) {
    sylva_value arg = args.values[i];
    assert_primitive(arg);
    sanitize_primitive(arg);
    switch (arg.type) {
    case sylva_type_integer: {
      //  float * integer
      if (context.type == sylva_type_float) {
        context.float_value /= arg.integer_value;
      }
      // integer * integer
      if (context.type == sylva_type_integer) {
        context.integer_value /= arg.integer_value;
      }
    }
      break;
    case sylva_type_float: {
      //  integer * float
      integer_to_float(context);
      //  float * float
      if (context.type == sylva_type_float) {
        context.float_value /= arg.float_value;
      }
    }
      break;
    default:break;
    }
  }
  return context;
}

sylva_value SYLVA_Number_I_mod(sylva_value context, sylva_args args) {
  assert_primitive(context);
  sanitize_primitive(context);
  assert(args.length == 1);
  sylva_value value = args.values[0];
  assert_primitive(value);
  sanitize_primitive(value);
  if (value.type == sylva_type_integer) {
    if (context.type == sylva_type_float) {
      return sylva_float_value(fmod(context.float_value, (sylva_float) value.integer_value));
    } else {
      return sylva_integer_value(context.integer_value % value.integer_value);
    }
  } else {
    if (context.type == sylva_type_float) {
      return sylva_float_value(fmod(context.float_value, value.float_value));
    } else {
      return sylva_integer_value(context.integer_value % (sylva_integer) value.float_value);
    }
  }
}

sylva_value SYLVA_Number_I_compare(sylva_value context, sylva_args args) {
  assert_primitive(context);
  sanitize_primitive(context);
  assert(args.length == 1);
  sylva_value value = args.values[0];
  assert_primitive(value);
  sanitize_primitive(value);
  if (value.type == sylva_type_integer) {
    if (context.type == sylva_type_float) {
      return sylva_integer_value(to_compare(context.float_value, value.integer_value));
    } else {
      return sylva_integer_value(to_compare(context.integer_value, value.integer_value));
    }
  } else {
    if (context.type == sylva_type_float) {
      return sylva_integer_value(to_compare(context.float_value, value.float_value));
    } else {
      return sylva_integer_value(to_compare(context.integer_value, value.float_value));
    }
  }
}

sylva_value SYLVA_Number_I_lt(sylva_value context, sylva_args args) {
  sylva_value result = SYLVA_Number_I_compare(context, args);
  return sylva_boolean_value(result.integer_value == sylva_descending);
}

sylva_value SYLVA_Number_I_lt_eq(sylva_value context, sylva_args args) {
  sylva_value result = SYLVA_Number_I_compare(context, args);
  return sylva_boolean_value(result.integer_value == sylva_descending || result.integer_value == sylva_same);
}

sylva_value SYLVA_Number_I_gt(sylva_value context, sylva_args args) {
  sylva_value result = SYLVA_Number_I_compare(context, args);
  return sylva_boolean_value(result.integer_value == sylva_ascending);
}

sylva_value SYLVA_Number_I_gt_eq(sylva_value context, sylva_args args) {
  sylva_value result = SYLVA_Number_I_compare(context, args);
  return sylva_boolean_value(result.integer_value == sylva_ascending || result.integer_value == sylva_same);
}

sylva_value SYLVA_Number_I_eq(sylva_value context, sylva_args args) {
  sylva_value result = SYLVA_Number_I_compare(context, args);
  return sylva_boolean_value(result.integer_value == sylva_same);
}

sylva_value SYLVA_Number_I_not_eq(sylva_value context, sylva_args args) {
  sylva_value result = SYLVA_Number_I_compare(context, args);
  return sylva_boolean_value(result.integer_value != sylva_same);
}

sylva_value SYLVA_Number_I_or(sylva_value context, sylva_args args) {
  assert_primitive(context);
  assert(args.length == 1);
  sylva_value value = args.values[0];
  return sylva_boolean_value(sylva_to_boolean(context) || sylva_to_boolean(value));
}

sylva_value SYLVA_Number_I_and(sylva_value context, sylva_args args) {
  assert_primitive(context);
  assert(args.length == 1);
  sylva_value value = args.values[0];
  return sylva_boolean_value(sylva_to_boolean(context) && sylva_to_boolean(value));
}

sylva_value SYLVA_Number_I_bit_or(sylva_value context, sylva_args args) {
  assert_primitive(context);
  assert(args.length == 1);
  sylva_value value = args.values[0];
  to_integer(context);
  to_integer(value);
  return sylva_integer_value(context.integer_value | value.integer_value);
}

sylva_value SYLVA_Number_I_bit_and(sylva_value context, sylva_args args) {
  assert_primitive(context);
  assert(args.length == 1);
  sylva_value value = args.values[0];
  to_integer(context);
  to_integer(value);
  return sylva_integer_value(context.integer_value & value.integer_value);
}

sylva_value SYLVA_Number_I_bit_xor(sylva_value context, sylva_args args) {
  assert_primitive(context);
  assert(args.length == 1);
  sylva_value value = args.values[0];
  to_integer(context);
  to_integer(value);
  return sylva_integer_value(context.integer_value ^ value.integer_value);
}

sylva_value SYLVA_Number_I_rshift(sylva_value context, sylva_args args) {
  assert_primitive(context);
  assert(args.length == 1);
  sylva_value value = args.values[0];
  to_integer(context);
  to_integer(value);
  return sylva_integer_value(context.integer_value >> value.integer_value);
}

sylva_value SYLVA_Number_I_lshift(sylva_value context, sylva_args args) {
  assert_primitive(context);
  assert(args.length == 1);
  sylva_value value = args.values[0];
  to_integer(context);
  to_integer(value);
  return sylva_integer_value(context.integer_value << value.integer_value);
}

sylva_value SYLVA_Number_I_abs(sylva_value context, sylva_args args) {
  assert_primitive(context);
  if (context.type == sylva_type_float) {
    return sylva_float_value(fabs(context.float_value));
  } else if (context.type == sylva_type_integer) {
    return sylva_integer_value(labs(context.integer_value));
  } else {
    return context;
  }
}

sylva_value SYLVA_Number_I_to_i(sylva_value context, sylva_args args) {
  assert_primitive(context);
  return sylva_integer_value(sylva_to_integer(context));
}

sylva_value SYLVA_Number_I_to_f(sylva_value context, sylva_args args) {
  assert_primitive(context);
  return sylva_float_value(sylva_to_float(context));
}

sylva_value SYLVA_Number_I_to_b(sylva_value context, sylva_args args) {
  assert_primitive(context);
  return sylva_boolean_value(sylva_to_boolean(context));
}

/***********************************************************************************************************************
 * Object
 **********************************************************************************************************************/

#define assert_is_object(V) assert((V).type == sylva_type_object)

sylva_class SYLVA_Object = {
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

sylva_value SYLVA_Object_I_init(sylva_value context, sylva_args args) {
  return context;
}

sylva_value SYLVA_Object_I_class(sylva_value context, sylva_args args) {
  assert_is_object(context);
  return sylva_class_value(context.object_value->class);
}
