//
// Created by Ryan on 2016/9/26.
//

#define __SYLVA_RUNTIME_SOURCE__
#include "sylva/runtime.h"

sl_value sl_func_call(sl_imp func, sl_value context, sl_index length, ...) {
  va_list list;
  va_start(list, length);
  sl_value result = sl_func_v_call(func, context, length, list);
  va_end(list);
  return result;
}

sl_value sl_func_v_call(sl_imp func, sl_value context, sl_index length, va_list list) {
  sl_args args = sl_args_v_create(length, list);
  sl_value result = func(context, args);
  sl_args_destroy(args);
  return result;
}

sl_imp sl_funcs_get(sl_funcs list, sl_symbol name) {
  for (sl_index i = 0; i < list.length; i++) {
    if (sl_symbol_equals(name, list.entries[i].name)) {
      return list.entries[i].imp;
    }
  }
  return NULL;
}

sl_imp sl_funcs_set(sl_funcs list, sl_symbol name, sl_imp func) {
  for (sl_index i = 0; i < list.length; i++) {
    if (sl_symbol_equals(name, list.entries[i].name)) {
      list.entries[i].imp = func;
      return func;
    }
  }
  return NULL;
}
