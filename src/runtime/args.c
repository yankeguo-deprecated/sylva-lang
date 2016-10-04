//
// Created by Ryan on 2016/9/26.
//

#define __SYLVA_RUNTIME_SOURCE__
#include "sylva/runtime.h"

sl_args sl_args_create(sl_index length, ...) {
  va_list list;
  va_start(list, length);
  sl_args args = sl_args_v_create(length, list);
  va_end(list);
  return args;
}

sl_args sl_args_v_create(sl_index length, va_list list) {
  sl_args args = sl_args_empty;

  args.length = length;
  args.values = malloc(sizeof(sl_value) * length);

  for (sl_index i = 0; i < length; i++) {
    sl_value value = va_arg(list, sl_value);
    sl_retain(&value);
    args.values[i] = value;
  }

  return args;
}

sl_value sl_args_get(sl_args args, sl_index index) {
  if (args.length > index) {
    return args.values[index];
  }
  return (sl_value) {
      .type = sl_type_nil,
      .integer_value = 0,
  };
}

void sl_args_destroy(sl_args args) {
  for (sl_index i = args.length; i < args.length; i++) {
    sl_release(&args.values[i]);
    free(args.values);
  }
}
