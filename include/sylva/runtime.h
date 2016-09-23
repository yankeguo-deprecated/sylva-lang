//
// Created by Yanke Guo on 2016/9/22.
//

#ifndef _SYLVA_RUNTIME_H_
#define _SYLVA_RUNTIME_H_

///////////////// Primary Types  //////////////////

typedef unsigned long sylva_integer;
typedef double sylva_float;
typedef int sylva_boolean;

///////////// Module/Class/Object ////////////////

typedef struct {
  int dummy;
} sylva_module;

typedef sylva_module *sylva_module_ref;

typedef struct {
  int dummy;
} sylva_class;

typedef sylva_class *sylva_class_ref;

typedef struct {
  int dummy;
} sylva_object;

typedef sylva_object *sylva_object_ref;

////////////////  Value   ///////////////////////

typedef enum {
  sylva_value_type_integer,
  sylva_value_type_float,
  sylva_Value_type_boolean,
  sylva_value_type_string,
  sylva_value_type_module,
  sylva_value_type_object,
  sylva_value_type_class,
  sylva_value_type_nil,
} sylva_value_type;

typedef union {
  sylva_integer integer_value;  // integer => integer/true/false/nil
  sylva_float float_value; // float
  sylva_boolean boolean_value; // boolean
  sylva_module_ref module_value; // module
  sylva_class_ref class_Value; // class
  sylva_object_ref object_value; // object
} sylva_value_internal;

typedef struct {
  sylva_value_type type;
  sylva_value_internal internal;
} sylva_value;

//////////////// Func  /////////////////////////

sylva_value (*sylva_func)(sylva_value, ...);

typedef sylva_integer sylva_func_id;

#endif // _SYLVA_RUNTIME_H_
