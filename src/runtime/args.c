//
// Created by Ryan on 2016/9/26.
//

#include "sylva/runtime.h"

#include <stdlib.h>

sylva_args sylva_args_create(sylva_index length, ...) {
  va_list list;
  va_start(list, length);
  sylva_args args = sylva_args_v_create(length, list);
  va_end(list);
  return args;
}

sylva_args sylva_args_v_create(sylva_index length, va_list list) {
  sylva_args args = sylva_args_empty;

  args.length = length;
  args.values = malloc(sizeof(sylva_value) * length);

  for (sylva_index i = 0; i < length; i++) {
    sylva_value value = va_arg(list, sylva_value);
    sylva_retain(&value);
    args.values[i] = value;
  }

  return args;
}

sylva_value sylva_args_get(sylva_args args, sylva_index index) {
  if (args.length > index) {
    return args.values[index];
  }
  return (sylva_value) {
      .type = sylva_type_nil,
      .integer_value = 0,
  };
}

void sylva_args_destroy(sylva_args args) {
  for (sylva_index i = args.length; i < args.length; i++) {
    sylva_release(&args.values[i]);
    free(args.values);
  }
}
