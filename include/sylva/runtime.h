//
// Created by Yanke Guo on 2016/9/22.
//

#ifndef _SYLVA_RUNTIME_H_
#define _SYLVA_RUNTIME_H_

#include "sylva/platform.h"

#include <stdarg.h>

__CPP_DECL_START

///////////////// Primary Types  //////////////////

typedef long sylva_integer;
typedef unsigned long sylva_index;
typedef double sylva_float;
typedef int sylva_boolean;

enum {
  sylva_index_not_found = ULONG_MAX,
};

enum {
  sylva_false = 0,
  sylva_true = 1,
};

///////////// Module/Class/Object/Value ////////////////

typedef struct sylva_module_t sylva_module;
typedef struct sylva_class_t sylva_class;
typedef struct sylva_object_t sylva_object;
typedef struct sylva_func_list_t sylva_func_list;
typedef struct sylva_member_list_t sylva_member_list;
typedef struct sylva_members_t sylva_members;

typedef sylva_module *sylva_module_ref;
typedef sylva_class *sylva_class_ref;
typedef sylva_object *sylva_object_ref;
typedef sylva_func_list *sylva_func_list_ref;
typedef sylva_member_list *sylva_member_list_ref;
typedef sylva_members *sylva_members_ref;

struct sylva_module_t {
  char *name;

  sylva_module_ref super; // pointer to super module

  sylva_func_list_ref static_func_list;
  sylva_func_list_ref instance_func_list;

  sylva_member_list_ref static_member_list;
  sylva_member_list_ref instance_member_list;
};

struct sylva_class_t {
  char *name;

  sylva_class_ref super; // pointer to super class

  sylva_module_ref *modules; // pointer to modules
  sylva_index modules_count; // number of modules

  sylva_func_list_ref static_func_list; // static func_list
  sylva_func_list_ref instance_func_list; // instance func_list

  sylva_member_list_ref static_member_list;
  sylva_member_list_ref instance_member_list;

  sylva_members_ref members;
};

struct sylva_object_t {
  sylva_class_ref class;

  sylva_members_ref members;
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

typedef struct {
  sylva_value_type type;
  union {
    sylva_integer integer_value;  // integer => integer/true/false/nil
    sylva_float float_value; // float
    sylva_boolean boolean_value; // boolean
    sylva_module_ref module_value; // module
    sylva_class_ref class_value; // class
    sylva_object_ref object_value; // object
  };
} sylva_value;

#define sylva_bare_value_nil {.type = sylva_value_type_nil, .integer_value = 0}
#define sylva_value_nil ((sylva_value) sylva_bare_value_nil)

#define sylva_bare_value_boolean(X) {.type = sylva_value_type_boolean, .integer_value = (X) != 0 }
#define sylva_value_boolean(X) ((sylva_value) sylva_bare_value_boolean(X))

#define sylva_bare_value_integer(X) {.type = sylva_value_type_integer, .integer_value = (X)}
#define sylva_value_integer(X) ((sylva_value) sylva_bare_value_integer(X))

#define sylva_bare_value_float(X) {.type = sylva_value_type_float, .float_value = (X)}
#define sylva_value_float(X) ((sylva_value) sylva_bare_value_float(X))

#define sylva_bare_value_object(X) {.type = sylva_value_type_object, .object_value = (X)}
#define sylva_value_object(X) ((sylva_value) sylva_bare_value_object(X))

#define sylva_bare_value_class(X) {.type = sylva_value_type_class, .class_value = (X)}
#define sylva_value_class(X) ((sylva_value) sylva_bare_value_class(X))

#define sylva_bare_value_module(X) {.type = sylva_value_type_module, .module_value = (X)}
#define sylva_value_module(X) ((sylva_value) sylva_bare_value_module(X))

//////////////// Func Arguments /////////////////////////

typedef struct {
  sylva_index length;
  sylva_value *values;
} sylva_args;

#define sylva_args_empty ((sylva_args){ .length = 0, .values = NULL })

typedef sylva_args *sylva_args_ref;

SYLVA_EXTERN sylva_value sylva_args_get(sylva_args args, sylva_index index);

SYLVA_EXTERN sylva_args sylva_args_create(sylva_index length, ...);

SYLVA_EXTERN sylva_args sylva_args_v_create(sylva_index length, va_list list);

SYLVA_EXTERN void sylva_args_destroy(sylva_args args);

////////////////     Func     /////////////////////////

/**
 * sylva_func is the general type for sylva functions
 *
 * for instance methods, context will be a sylva_object_ref, for static, it will be sylva_class_ref
 */
typedef sylva_value (*sylva_func)(sylva_value context, sylva_args args);

typedef sylva_index sylva_func_id;

/**
 * call a sylva_func
 */
SYLVA_EXTERN sylva_value sylva_func_call(sylva_func func, sylva_value context, sylva_index length, ...);

/**
 * call a sylva_func with va_list as argument
 */
SYLVA_EXTERN sylva_value sylva_func_v_call(sylva_func func, sylva_value context, sylva_index length, va_list list);

enum {
  sylva_func_id_not_found = sylva_index_not_found,
};

/**
 * sylva_func_list stores a sylva_func_id to sylva_func map
 *
 * In an ideal situation, sylva_func_list should be initialized literally
 */
struct sylva_func_list_t {
  sylva_index length;
  sylva_func_id *func_ids;
  sylva_func *funcs;
};

SYLVA_EXTERN sylva_func sylva_func_list_get(sylva_func_list list, sylva_func_id func_id);

/**
 * Check whether a sylva_func_id existed in a sylva_func_list, but the sylva_func mapped maybe NULL
 *
 * @param list sylva_func_list
 * @param func_id sylva_func_id
 * @return sylva_true or sylva_false
 */
SYLVA_EXTERN sylva_boolean sylva_func_list_exists(sylva_func_list list, sylva_func_id func_id);

/**
 * Set a sylva_func to sylva_func_id, a NULL sylva_func will not remove sylva_func_id from sylva_func_list
 *
 * @param list sylva_func_list
 * @param func_id sylva_func_id
 * @param func sylva_func or NULL
 * @return NULL if out of length, func if succeeded
 */
SYLVA_EXTERN sylva_func sylva_func_list_set(sylva_func_list list, sylva_func_id func_id, sylva_func func);

SYLVA_EXTERN sylva_func sylva_class_instance_func_resolve(sylva_class class, sylva_func_id func_id);

SYLVA_EXTERN sylva_func sylva_class_static_func_resolve(sylva_class class, sylva_func_id func_id);

/**
 * resolve a sylva_func from sylva_func_id and a context
 *
 * !!WARN!!, this will not agressively resolve instance func to static func
 *
 */
SYLVA_EXTERN sylva_func sylva_func_resolve(sylva_value context, sylva_func_id func_id);

/**
 * resolve a sylva_func with context, and invoke it
 */
SYLVA_EXTERN sylva_value sylva_call(sylva_value context, sylva_func_id func_id, sylva_index length, ...);

/**
 * resolve a sylva_func with context, and invoke it, va_list version
 */
SYLVA_EXTERN sylva_value sylva_v_call(sylva_value context, sylva_func_id func_id, sylva_index length, va_list list);

///////////////////// FuncId Registry /////////////////

typedef struct {
  sylva_index length;
  sylva_func_id *func_ids;
  char **func_names;
} sylva_func_id_registry;

typedef sylva_func_id_registry *sylva_func_id_registry_ref;

/**
 * The Global FuncId Registry, should be provided by compile target
 *
 * In ideal situation, this global value should be initialized literally
 */
extern sylva_func_id_registry sylva_runtime_func_id_registry;

SYLVA_EXTERN char *sylva_func_id_registry_get_func_name(sylva_func_id_registry registry, sylva_func_id func_id);

SYLVA_EXTERN sylva_func_id sylva_func_id_registry_get_func_id(sylva_func_id_registry registry, char *func_name);

SYLVA_EXTERN char *sylva_runtime_func_id_to_name(sylva_func_id func_id);

SYLVA_EXTERN sylva_func_id sylva_runtime_func_name_to_id(char *func_name);

/////////////////////// Member /////////////////////

typedef sylva_index sylva_member_id;

enum {
  sylva_member_id_not_found = sylva_index_not_found,
};

struct sylva_member_list_t {
  sylva_index length;
  sylva_member_id *member_ids;
};

struct sylva_members_t {
  sylva_index length;
  sylva_member_id *member_ids;
  sylva_value *member_values;
};

//////////////////// MemberId Registry /////////////////

typedef struct {
  sylva_index length;
  sylva_member_id *member_ids;
  char **member_names;
} sylva_member_id_registry;

typedef sylva_member_id_registry *sylva_member_id_registry_ref;

/**
 * The Global FuncId Registry, should be provided by compile target
 *
 * In ideal situation, this global value should be initialized literally
 */
extern sylva_member_id_registry sylva_runtime_member_id_registry;

SYLVA_EXTERN char *sylva_member_id_registry_get_func_name(sylva_member_id_registry registry, sylva_member_id member_id);

SYLVA_EXTERN sylva_member_id sylva_member_id_registry_get_func_id(sylva_member_id_registry registry, char *member_name);

SYLVA_EXTERN char *sylva_runtime_member_id_to_name(sylva_member_id member_id);

SYLVA_EXTERN sylva_member_id sylva_runtime_member_name_to_id(char *member_name);

__CPP_DECL_END

#endif // _SYLVA_RUNTIME_H_
