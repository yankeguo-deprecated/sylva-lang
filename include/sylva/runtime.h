//
// Created by Yanke Guo on 2016/9/22.
//

#ifndef _SYLVA_RUNTIME_H_
#define _SYLVA_RUNTIME_H_

#include "sylva/platform.h"

///////////////// Primary Types  //////////////////

typedef long sylva_integer;
typedef unsigned long sylva_index;
typedef double sylva_float;
typedef int sylva_boolean;

enum {
  sylva_false = 0,
  sylva_true = 1,
};

///////////// Module/Class/Object/Value ////////////////

typedef struct sylva_module_t sylva_module;
typedef struct sylva_class_t sylva_class;
typedef struct sylva_object_t sylva_object;
typedef struct sylva_func_list_t sylva_func_list;

typedef sylva_module *sylva_module_ref;
typedef sylva_class *sylva_class_ref;
typedef sylva_object *sylva_object_ref;
typedef sylva_func_list *sylva_func_list_ref;

struct sylva_module_t {
  sylva_module_ref super; // pointer to super module

  sylva_func_list_ref static_func_list;
  sylva_func_list_ref instance_func_list;
};

struct sylva_class_t {
  sylva_class_ref super; // pointer to super class

  sylva_module_ref modules; // pointer to modules
  sylva_index modules_count; // number of modules

  sylva_func_list_ref static_func_list; // static func_list
  sylva_func_list_ref instance_func_list; // instance func_list
};

struct sylva_object_t {
  sylva_class_ref class;
};

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

typedef sylva_value (*sylva_func)(sylva_value, ...);

typedef sylva_index sylva_func_id;

/**
 * sylva_func_list stores a sylva_func_id to sylva_func map
 *
 * In an ideal situation, sylva_func_list should be initialized literally
 */
struct sylva_func_list_t {
  sylva_index capacity;
  sylva_func_id *func_ids;
  sylva_func *funcs;
};

SYLVA_EXPORT sylva_func sylva_func_list_get(sylva_func_list list, sylva_func_id func_id);

/**
 * Check whether a sylva_func_id existed in a sylva_func_list, but the sylva_func mapped maybe NULL
 *
 * @param list sylva_func_list
 * @param func_id sylva_func_id
 * @return sylva_true or sylva_false
 */
SYLVA_EXPORT sylva_boolean sylva_func_list_exists(sylva_func_list list, sylva_func_id func_id);

/**
 * Set a sylva_func to sylva_func_id, a NULL sylva_func will not remove sylva_func_id from sylva_func_list
 *
 * @param list sylva_func_list
 * @param func_id sylva_func_id
 * @param func sylva_func or NULL
 * @return NULL if out of capacity, func if succeeded
 */
SYLVA_EXPORT sylva_func sylva_func_list_set(sylva_func_list list, sylva_func_id func_id, sylva_func func);

///////////////////// FuncId Registry /////////////////

#endif // _SYLVA_RUNTIME_H_
