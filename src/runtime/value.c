//
// Created by Ryan on 2016/9/26.
//

#include "sylva/runtime.h"

#include <stdlib.h>

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
