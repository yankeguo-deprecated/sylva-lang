//
// Created by Ryan on 2016/9/26.
//

#include "sylva/runtime.h"

#include <stdlib.h>
#include <string.h>

sylva_value sylva_func_call(sylva_imp func, sylva_value context, sylva_index length, ...) {
  va_list list;
  va_start(list, length);
  sylva_value result = sylva_func_v_call(func, context, length, list);
  va_end(list);
  return result;
}

sylva_value sylva_func_v_call(sylva_imp func, sylva_value context, sylva_index length, va_list list) {
  sylva_args args = sylva_args_v_create(length, list);
  sylva_value result = func(context, args);
  sylva_args_destroy(args);
  return result;
}

sylva_imp sylva_funcs_get(sylva_funcs list, sylva_symbol name) {
  for (sylva_index i = 0; i < list.length; i++) {
    if (sylva_symbol_equals(name, list.entries[i].name)) {
      return list.entries[i].imp;
    }
  }
  return NULL;
}

sylva_imp sylva_funcs_set(sylva_funcs list, sylva_symbol name, sylva_imp func) {
  for (sylva_index i = 0; i < list.length; i++) {
    if (sylva_symbol_equals(name, list.entries[i].name)) {
      list.entries[i].imp = func;
      return func;
    }
  }
  return NULL;
}
