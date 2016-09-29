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
 * Number
 **********************************************************************************************************************/

#define assert_is_number(V)\
  assert((V).type == sylva_type_integer ||\
  (V).type == sylva_type_float ||\
  (V).type == sylva_type_boolean)\

SYLVA_WEAK sylva_class SYLVA_Number = {
    .name = "Number",
    .super = NULL,
    .static_funcs = NULL,
    .instance_funcs = NULL,
    .static_member_list = NULL,
    .instance_member_list = NULL,
    .members = NULL,
};

sylva_value SYLVA_Number_I_not(sylva_value context, sylva_args args) {
  assert_is_number(context);
  return sylva_value_boolean(!sylva_to_boolean(context));
}

sylva_value SYLVA_Number_I_add(sylva_value context, sylva_args args) {
  assert_is_number(context);
  if (context.type == sylva_type_boolean) {
    context.type = sylva_type_integer;
    context.integer_value = context.boolean_value ? 1 : 0;
  }
  for (sylva_index i = 0; i < args.length; i++) {
    sylva_value arg = args.values[i];
    if (arg.type == sylva_type_boolean) {
      arg.type = sylva_type_integer;
      arg.integer_value = arg.boolean_value ? 1 : 0;
    }
    assert_is_number(arg);
    switch (arg.type) {
    case sylva_type_integer: {
      if (context.type == sylva_type_float)
        context.float_value = context.float_value + arg.integer_value;
      if (context.type == sylva_type_integer)
        context.integer_value = context.integer_value + arg.integer_value;
      break;
    }
    case sylva_type_float: {
      //  Switch result to float value
      if (context.type == sylva_type_integer) {
        context.type = sylva_type_float;
        context.float_value = (sylva_float) context.integer_value;
      }
      if (context.type == sylva_type_float)
        context.float_value = context.float_value + arg.float_value;
      break;
    }
    case sylva_type_boolean: {
    }
    default:break;
    }
  }
  return context;
}

sylva_value SYLVA_Number_I_sub(sylva_value context, sylva_args args) {
  assert_is_number(context);

  if (args.length == 0) {
    switch (context.type) {
    case sylva_type_boolean: {
      context.type = sylva_type_integer;
      context.integer_value = -context.boolean_value;
      break;
    }
    case sylva_type_integer: {
      context.integer_value = -context.integer_value;
      break;
    }
    case sylva_type_float: {
      context.float_value = -context.float_value;
      break;
    }
    default:break;
    }
    return context;
  } else {
    for (sylva_index i = 0; i < args.length; i++) {
      sylva_args add_args = {
          .length = 1,
          .values = (sylva_value[]) {
              SYLVA_Number_I_sub(args.values[i], sylva_args_empty)
          }
      };
      context = SYLVA_Number_I_add(context, add_args);
    }
    return context;
  }
}

sylva_value SYLVA_Number_I_abs(sylva_value context, sylva_args args) {
  assert_is_number(context);
  if (context.type == sylva_type_float) {
    return sylva_value_float(fabs(context.float_value));
  } else if (context.type == sylva_type_integer) {
    return sylva_value_integer(labs(context.integer_value));
  } else {
    return context;
  }
}

sylva_value SYLVA_Number_I_to_i(sylva_value context, sylva_args args) {
  assert_is_number(context);
  return sylva_value_integer(sylva_to_integer(context));
}

sylva_value SYLVA_Number_I_to_f(sylva_value context, sylva_args args) {
  assert_is_number(context);
  return sylva_value_float(sylva_to_float(context));
  return context;
}

sylva_value SYLVA_Number_I_to_b(sylva_value context, sylva_args args) {
  assert_is_number(context);
  return sylva_value_boolean(sylva_to_boolean(context));
}

/***********************************************************************************************************************
 * Object
 **********************************************************************************************************************/

#define assert_is_object(V) assert((V).type == sylva_type_object)

SYLVA_WEAK sylva_class SYLVA_Object = {
    .name = "Object",
    .super = NULL,
    .static_funcs = NULL,
    .instance_funcs = NULL,
    .static_member_list = NULL,
    .instance_member_list = NULL,
    .members = NULL,
};

sylva_value SYLVA_Object_I_class(sylva_value context, sylva_args args) {
  assert_is_object(context);
  return sylva_value_class(context.object_value->class);
}
