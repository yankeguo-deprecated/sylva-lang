//
// Created by Yanke Guo on 2016/9/22.
//

#ifndef _SYLVA_RUNTIME_H_
#define _SYLVA_RUNTIME_H_

#include "sylva/platform.h"

#include <assert.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

__CPP_DECL_START

/***********************************************************************************************************************
 * Primitive Types
 **********************************************************************************************************************/

/**
 * boolean type in sylva runtime, alias to C `bool`
 */
typedef bool sylva_boolean;

/**
 * true value for boolean type
 */
#define sylva_true true
#define sylva_yes  sylva_true

/**
 * false value for boolean type
 */
#define sylva_false false
#define sylva_no    sylva_false

/**
 * index type, i.e. unsigned integer type, in sylva runtime, alias to C `unsigned long`
 *
 * this is also the type for function id and member id
 */
typedef unsigned long sylva_index;

/**
 * max possible sylva_index value
 */
#define sylva_index_max ULONG_MAX

/**
 * index type returned when nothing is found, alias to max value for unsigned long
 */
#define sylva_index_not_found sylva_index_max

/**
 * integer type in sylva runtime, alias to C `long`
 */
typedef long sylva_integer;

/**
 * float type in sylva runtime, alias to C `double`
 */
typedef double sylva_float;

/**
 * comparison result
 */
typedef enum {
  /**
   * left < right
   */
      sylva_ascending = -1,
  /**
   * left == right
   */
      sylva_same = 0,
  /**
   * left > right
   */
      sylva_descending = 1,
} sylva_comparison_result;

/**
 * compare two values and returns sylva_compare_result
 */
#define sylva_compare(A, B) ((A) == (B) ? sylva_same : ((A) > (B) ? sylva_descending : sylva_ascending))

/**
 * char* is used as symbole in sylva, used for identify a func or a member variable
 *
 * symbol should not be created dynamically
 */
typedef char *const sylva_symbol;

/**
 * check if two symbols are same
 */
#define sylva_symbol_equals(A, B) (strcmp((A), (B)) == 0)

/***********************************************************************************************************************
 * Assert
 **********************************************************************************************************************/

#define sylva_assert(CLAUSE, ...) if (!(CLAUSE)) { fprintf(stderr, __VA_ARGS__); abort(); }

/***********************************************************************************************************************
 * Forward Declarations
 **********************************************************************************************************************/

/*
 * Struct Declarations
 */
typedef struct sylva_class_t sylva_class;
typedef struct sylva_object_t sylva_object;
typedef struct sylva_funcs_t sylva_funcs;
typedef struct sylva_member_defs_t sylva_member_defs;
typedef struct sylva_members_t sylva_members;
typedef struct sylva_args_t sylva_args;

/*
 * Pointer Declarations
 */
typedef sylva_class *sylva_class_ref;
typedef sylva_object *sylva_object_ref;
typedef sylva_funcs *sylva_funcs_ref;
typedef sylva_member_defs *sylva_member_defs_ref;
typedef sylva_members *sylva_members_ref;

/***********************************************************************************************************************
 * Class & Object
 **********************************************************************************************************************/

/*
 * !!WARN!! Module in sylva-lang is a compile-time only concept
 */

/**
 * Class in sylva-lang
 *
 * for best performance, every sylva_class is statically initizlied in compile result
 */
struct sylva_class_t {
  /**
   * name of class
   */
  sylva_symbol name;

  /**
   * pointer to superclass
   */
  sylva_class_ref super;

  /**
   * static functions decalred by this class
   */
  sylva_funcs_ref static_funcs;

  /**
   * instance functions declared by this class
   */
  sylva_funcs_ref instance_funcs;

  /**
   * static member variables declared by this class
   */
  sylva_member_defs_ref static_member_defs;

  /**
   * instance member variables declared by this class
   */
  sylva_member_defs_ref instance_member_defs;

  /**
   * storage for all static member variables
   */
  sylva_members_ref members;

  /**
   * deinitializer (sylva_func)
   */
  void *deinitializor;
};

/**
 * Object in sylva-lang
 *
 * every object in sylva is dynamically initialized, except some special classes (String)
 */
struct sylva_object_t {
  /**
   * pointer to class
   */
  sylva_class_ref class;
  /**
   * storage for instance variables, including variables declared in superclass
   */
  sylva_members_ref members;
  /**
   * reference counter, used for garbage collection
   */
  sylva_index ref_count;
  /**
   * deinitializing
   */
  sylva_boolean deinitializing;
};

/**
 * allocate memory for an object of class, including storage for member variables
 *
 * @return pointer to created object
 */
SYLVA_EXTERN sylva_object_ref sylva_object_create(sylva_class_ref class);

/**
 * increase reference counter of a object
 *
 * @param object pointer to target object
 */
SYLVA_EXTERN sylva_object_ref sylva_object_retain(sylva_object_ref object);

/**
 * decrease reference counter of a object, if reaches 0, destroy it
 *
 * @param object pointer to target object
 */
SYLVA_EXTERN sylva_object_ref sylva_object_release(sylva_object_ref object);

/**
 * destroy a object
 *
 * free all memory allocated for object, including storage for member variables
 *
 * @param object pointer to target object
 */
SYLVA_EXTERN void sylva_object_destroy(sylva_object_ref object);

/***********************************************************************************************************************
 * Mixin Value
 **********************************************************************************************************************/

/**
 * enum for sylva value types
 */
typedef enum {
  /**
   * nil type for sylva
   */
      sylva_type_nil = 1,
  /**
   * type for sylva_boolean
   */
      sylva_type_boolean = 1 << 1,
  /**
   * type for sylva_integer
   */
      sylva_type_integer = 1 << 2,
  /**
   * type for sylva_float
   */
      sylva_type_float = 1 << 3,
  /**
   * sum all numeric types
   */
      sylva_type_numeric = sylva_type_integer | sylva_type_float,
  /**
   * sum all primitive types
   */
      sylva_type_primitive = sylva_type_nil | sylva_type_boolean | sylva_type_integer | sylva_type_float,
  /**
   * type for sylva_object_ref
   */
      sylva_type_object = 1 << 4,
  /**
   * type for sylva_class_ref
   */
      sylva_type_class = 1 << 5,
  /**
   * type for wild C pointer, sylva-runtime will never use this type, reserved for native bindings
   */
      sylva_type_pointer = 1 << 6,

} sylva_value_type;

/**
 * Value for sylva-lang, it's a mixin, support all types used in sylva-lang
 */
typedef struct {
  /**
   * type for this sylva_value
   */
  sylva_value_type type;

  union {
    /**
     * value as sylva_integer
     */
    sylva_integer integer_value;
    /**
     * value for sylva_float
     */
    sylva_float float_value;
    /**
     * value for sylva_boolean
     */
    sylva_boolean boolean_value;
    /**
     * value for sylva_class pointer
     */
    sylva_class_ref class_value;
    /**
     * value for sylva_object pointer
     */
    sylva_object_ref object_value;
    /**
     * value for C pointer, will be same as class_value and object_value
     */
    void *pointer_value;
  };
} sylva_value;

/**
 * pointer to Value
 */
typedef sylva_value *sylva_value_ref;

/**
 * bare sylva_value initializer macros, used for statically init
 */
#define sylva_bare_pointer_value(X) {.type = sylva_type_pointer, .pointer_value = (X)}
#define sylva_bare_nil_value {.type = sylva_type_nil, .integer_value = 0}
#define sylva_bare_boolean_value(X) {.type = sylva_type_boolean, .boolean_value = (X)}
#define sylva_bare_true_value {.type = sylva_type_boolean, .boolean_value = sylva_true}
#define sylva_bare_false_value {.type = sylva_type_boolean, .boolean_value = sylva_false}
#define sylva_bare_integer_value(X) {.type = sylva_type_integer, .integer_value = (X)}
#define sylva_bare_float_value(X) {.type = sylva_type_float, .float_value = (X)}
#define sylva_bare_object_value(X) {.type = sylva_type_object, .object_value = (X)}
#define sylva_bare_class_value(X) {.type = sylva_type_class, .class_value = (X)}

/**
 * sylva_value initializer macros
 */
#define sylva_pointer_value(X)  ((sylva_value) sylva_bare_pointer_value(X))
#define sylva_nil_value ((sylva_value) sylva_bare_nil_value)
#define sylva_boolean_value(X) ((sylva_value) sylva_bare_boolean_value(X))
#define sylva_true_value sylva_boolean_value(sylva_true)
#define sylva_false_value sylva_boolean_value(sylva_false)
#define sylva_integer_value(X) ((sylva_value) sylva_bare_integer_value(X))
#define sylva_float_value(X) ((sylva_value) sylva_bare_float_value(X))
#define sylva_object_value(X) ((sylva_value) sylva_bare_object_value(X))
#define sylva_class_value(X) ((sylva_value) sylva_bare_class_value(X))

/**
 * turns any sylva_value to a resonable boolean value
 *
 * @param value sylva_value
 *
 * @return sylva_true or sylva_false
 */
SYLVA_EXTERN sylva_boolean sylva_to_boolean(sylva_value value);

/**
 * turns any sylva_value to a resonable integer value
 *
 * @param value sylva_value
 *
 * @return sylva_value with integer value type
 */
SYLVA_EXTERN sylva_integer sylva_to_integer(sylva_value value);

/**
 * turns any sylva_value to a resonable integer value
 *
 * @param value sylva_value
 *
 * @return sylva_value with integer value type
 */
SYLVA_EXTERN sylva_float sylva_to_float(sylva_value value);

/**
 * turns to number values
 *
 * @param value pointer to value to convert
 */
SYLVA_EXTERN void sylva_trans_to_numeric(sylva_value_ref value);

/**
 * turns to float
 *
 * @param value pointer to value to convert
 */
SYLVA_EXTERN void sylva_trans_to_float(sylva_value_ref value);

/**
 * turns to integer
 *
 * @param value pointer to value to convert
 */
SYLVA_EXTERN void sylva_trans_to_integer(sylva_value_ref value);

/**
 * turns to boolean
 *
 * @param value pointer to value to convert
 */
SYLVA_EXTERN void sylva_trans_to_boolean(sylva_value_ref value);

/***********************************************************************************************************************
 * Function Arguments
 **********************************************************************************************************************/

/**
 * Arguments for function invocation
 */
struct sylva_args_t {
  /**
   * number of arguments
   */
  sylva_index length;
  /**
   * storage for argument values
   */
  sylva_value *values;
};

/**
 * pointer to Arguments
 */
typedef sylva_args *sylva_args_ref;

/**
 * initializer macro for sylva_args
 */
#define sylva_args_make(C, ...) ((sylva_args){ .length = C, .values = (sylva_value[]){ __VA_ARGS__ }})
#define sylva_args_empty ((sylva_args){ .length = 0, .values = NULL })

/**
 * create a sylva_args with length
 *
 * @param length number of arguments
 * @param list arguments stored in va_list
 *
 * @return sylva_args
 */
SYLVA_EXTERN sylva_args sylva_args_create(sylva_index length, ...);
SYLVA_EXTERN sylva_args sylva_args_v_create(sylva_index length, va_list list);

/**
 * get a value at index from sylva_args
 *
 * @param args sylva_args
 * @param index index of value to get
 *
 * @return sylva_value at index
 */
SYLVA_EXTERN sylva_value sylva_args_get(sylva_args args, sylva_index index);

/**
 * destroy a previousely created sylva_args
 *
 * @param sylva_args to destroy
 */
SYLVA_EXTERN void sylva_args_destroy(sylva_args args);

/***********************************************************************************************************************
 * Function
 **********************************************************************************************************************/

/**
 * sylva_func is the general function type in sylva-lang, including instance functions and static functions
 *
 * !!WARN!! context may be those of subclasses
 *
 * @param context for instance functions this is the object, for class functions this is the class
 * @param args arguments passed in, will be automatically destroyed
 */
typedef sylva_value (*sylva_imp)(sylva_value context, sylva_args args);

/**
 * invoke a sylva_func, with context and variable arguments
 *
 * @param func function to invoke
 * @param context context for function, basically sylva_object or sylva_class
 * @param length number of arguments
 * @param list va_list
 *
 * @return result of function invocation
 */
SYLVA_EXTERN sylva_value sylva_func_call(sylva_imp func, sylva_value context, sylva_index length, ...);
SYLVA_EXTERN sylva_value sylva_func_v_call(sylva_imp func, sylva_value context, sylva_index length, va_list list);

/***********************************************************************************************************************
 * Function Storage
 **********************************************************************************************************************/

/**
 * function storage entry
 */
typedef struct sylva_func_t {
  /**
   * function name
   */
  char *name;

  /**
   * function implementation
   */
  sylva_imp imp;

} sylva_func;

#define sylva_func_item(NAME, IMP) { .name = NAME, .imp = IMP }
#define sylva_func_make(NAME, IMP) ((sylva_func) sylva_func_item(NAME, IMP))

/**
 * pointer type for sylva_funcs_entry
 */
typedef sylva_func *sylva_func_ref;

/**
 * sylva_funcs stores a sylva_symbol to sylva_func map
 *
 * !!WARN!! in an ideal situation, sylva_funcs should be initialized literally
 */
struct sylva_funcs_t {
  /**
   * number of functions
   */
  sylva_index length;

  /**
   * entries
   */
  sylva_func_ref entries;
};

#define sylva_funcs_make(L, ...) ((sylva_funcs) { .length = L, .entries = (sylva_func[]) { __VA_ARGS__ }})

/**
 * find a pointer of function from sylva_funcs by function id
 *
 * @param list sylva_funcs to find from
 * @param func_id function id to find
 *
 * @return pointer to function
 */
SYLVA_EXTERN sylva_imp sylva_funcs_get(sylva_funcs list, sylva_symbol name);

/**
 * set a sylva_func to a function id, a NULL sylva_func will not remove function id from sylva_funcs
 *
 * @param list sylva_funcs to set
 * @param func_id function id to set
 * @param func pointer to function to set
 *
 * @return NULL if out of length, func if succeeded
 */
SYLVA_EXTERN sylva_imp sylva_funcs_set(sylva_funcs list, sylva_symbol name, sylva_imp func);

/***********************************************************************************************************************
 * Member Viarable Declarations & Storage
 **********************************************************************************************************************/

/**
 * option for member variable
 */
typedef enum {
  /**
   * normal
   */
      sylva_member_normal = 0,
  /**
   * weak variable, will retain/release while setting
   */
      sylva_member_weak = 1 << 0,

} sylva_member_option;

/**
 * member definition
 */
typedef struct {
  /**
   * name
   */
  char *name;
  /**
   * option
   */
  sylva_member_option option;
} sylva_member_def;

typedef sylva_member_def *sylva_member_def_ref;

#define sylva_member_def_item(NAME, OPTION) { .name = NAME, .option = OPTION }
#define sylva_member_def_make(NAME, OPTION) ((sylva_member_def) sylva_member_def_item(NAME, OPTION))

/**
 * entry for members storage
 */
typedef struct sylva_member_t {
  /**
   * name
   */
  char *name;
  /**
   * option
   */
  sylva_member_option option;
  /**
   * value
   */
  sylva_value value;
} sylva_member;

#define sylva_member_item(NAME, OPTION) { .name = NAME, .option = OPTION, .value = sylva_bare_nil_value }
#define sylva_member_make(NAME, OPTION) ((sylva_member) sylva_member_item(NAME, OPTION))

/**
 * pointer
 */
typedef sylva_member *sylva_members_entry_ref;

/**
 * Member Viarable Declarations
 *
 * !!WARN!! in ideal situation, member viarable is declared statically
 */
struct sylva_member_defs_t {
  /**
   * number of members
   */
  sylva_index length;
  /**
   * member ids
   */
  sylva_member_def_ref entries;
};

#define sylva_member_defs_make(L, ...) ((sylva_member_defs) { .length = L, .entries = (sylva_member_def[]){ __VA_ARGS__ }})

/**
 * Member Storage
 *
 * !!WARN!! in ideal situation, class
 */
struct sylva_members_t {
  /**
   * number of members
   */
  sylva_index length;
  /**
   * entries
   */
  sylva_members_entry_ref entries;
};

#define sylva_members_make(L, ...) ((sylva_members) { .length = L, .entries = (sylva_member[]) { __VA_ARGS__ }})

/**
 * allocate memory for member storage with length provided
 *
 * @param length number of members
 *
 * @return pointer of allocated member storage
 */
SYLVA_EXTERN sylva_members_ref sylva_members_create(sylva_index length);

/**
 * init a members storage with members declared in a member list
 *
 * @param members pointer to member storage
 * @param start_idx from which index the member storage should init
 * @param member_list the member list should init from
 *
 * @return number of members inited
 */
SYLVA_EXTERN sylva_index sylva_members_init(sylva_members_ref members, sylva_index start_idx, sylva_member_defs defs);

/**
 * get a value from member storage with member id
 *
 * @param members pointer to member storage
 * @param member_id member id to get
 *
 * @return member value, sylva_value_nil if not found
 */
SYLVA_EXTERN sylva_value sylva_members_get(sylva_members members, sylva_symbol name);

/**
 * set a value to member storage with member id
 *
 * !!WARN!! this will invoke retain/release automatically
 *
 * @param members pointer to member storage
 * @param member_id member id to set
 * @param value value to set, will retain if this is not a weak member
 *
 * @param sylva_true if set, sylva_false if member id not found
 */
SYLVA_EXTERN sylva_boolean sylva_members_set(sylva_members members, sylva_symbol name, sylva_value value);

/**
 * destroy a member storage, will free everything and release all members
 */
SYLVA_EXTERN void sylva_members_destroy(sylva_members_ref members);

/***********************************************************************************************************************
 * Object-Oriented
 **********************************************************************************************************************/


/**
 * Get the corresponding class value from a value, also resolves virtual class for primitive values
 *
 * @param value incoming value
 * @return class associated
 */
SYLVA_EXTERN sylva_class_ref sylva_get_class(sylva_value value);

/**
 * resolve a function id to instance function from a Class, including superclass
 *
 * @param class Class to resolve from
 * @param func_id function id to resolve
 *
 * @return pointer to function if resolved, NULL or not
 */
SYLVA_EXTERN sylva_imp sylva_class_instance_func_resolve(sylva_class class, sylva_symbol name);

/**
 * resolve a function id to static function from a Class, including superclass
 *
 * @param class Class to resolve from
 * @param func_id function id to resolve
 *
 * @return pointer to function if resolved, NULL or not
 */
SYLVA_EXTERN sylva_imp sylva_class_static_func_resolve(sylva_class class, sylva_symbol name);

/**
 * resolve a function id to function from a Class or an Object, including superclass
 *
 * !!WARN!! this will not agressively resolve instance function id to static function
 *
 * @param context Class or Object to resolve from
 * @param func_id function id to resolve
 *
 * @return pointer to function if resolved, NULL or not
 */
SYLVA_EXTERN sylva_imp sylva_func_resolve(sylva_value context, sylva_symbol name);

/**
 * resolve a function id to function from superclass of a Class or an Object's class, including superclass
 *
 * !!WARN!! this will not agressively resolve instance function id to static function
 *
 * @param context Class or Object to resolve from
 * @param class pointer of base class to search from, excluding
 * @param func_id function id to resolve
 *
 * @return pointer to function if resolved, NULL or not
 */
SYLVA_EXTERN sylva_imp sylva_func_resolve_super(sylva_value context, sylva_class_ref class, sylva_symbol name);

/**
 * resolve a function with a Class or an Object, and invoke it
 *
 * @param context Class/Object to invoke
 * @param func_id function id to invoke
 * @param length number of arguments
 * @param list arguments stored in va_list
 *
 * @return function invocation result
 */
SYLVA_EXTERN sylva_value sylva_call(sylva_value context, sylva_symbol name, sylva_index length, ...);
SYLVA_EXTERN sylva_value sylva_v_call(sylva_value context, sylva_symbol name, sylva_index length, va_list list);

/**
 * resolve a function with superclass of a Class or an Object, and invoke it
 *
 * @param context Class/Object to invoke
 * @param func_id function id to invoke
 * @param length number of arguments
 * @param list arguments stored in va_list
 *
 * @return function invocation result
 */
SYLVA_EXTERN sylva_value sylva_call_super(sylva_value context,
                                          sylva_class_ref class,
                                          sylva_symbol name,
                                          sylva_index length,
                                          ...);
SYLVA_EXTERN sylva_value sylva_v_call_super(sylva_value context,
                                            sylva_class_ref class,
                                            sylva_symbol name,
                                            sylva_index length,
                                            va_list list);

/**
 * create a instance of a class and initialize with specifed initializer
 *
 * @param class class to intialize
 * @param func_id function id to initialize
 * @param length number of arguments
 * @param list arguments in va_list
 */
SYLVA_EXTERN sylva_value sylva_create(sylva_class_ref class, sylva_symbol name, sylva_index length, ...);

SYLVA_EXTERN sylva_value sylva_v_create(sylva_class_ref class, sylva_symbol name, sylva_index length, va_list list);

/**
 * invoke sylva_object_retain if value is an object
 *
 * do nothing if value is not an object
 *
 * @param value sylva_value to ratain
 */
SYLVA_EXTERN void sylva_retain(sylva_value_ref value);

/**
 * invoke sylva_object_release if value is an object
 *
 * do nothing if value is not a object
 *
 * @param value sylva_value to ratain
 */
SYLVA_EXTERN void sylva_release(sylva_value_ref value);

/**
 * get member from object, class and value(object/class type)
 *
 * @see sylva_members_get
 */
SYLVA_EXTERN sylva_value sylva_object_members_get(sylva_object object, sylva_symbol name);
SYLVA_EXTERN sylva_value sylva_class_members_get(sylva_class class, sylva_symbol name);
SYLVA_EXTERN sylva_value sylva_get(sylva_value value, sylva_symbol name);
SYLVA_EXTERN sylva_value sylva_static_get(sylva_value value, sylva_symbol name);

/**
 * set member to object, class and value(object/class type)
 *
 * @see sylva_members_set
 */
SYLVA_EXTERN sylva_boolean sylva_object_members_set(sylva_object object, sylva_symbol name, sylva_value value);
SYLVA_EXTERN sylva_boolean sylva_class_members_set(sylva_class class, sylva_symbol name, sylva_value value);
SYLVA_EXTERN sylva_boolean sylva_set(sylva_value target_value, sylva_symbol name, sylva_value value);
SYLVA_EXTERN sylva_boolean sylva_static_set(sylva_value target_value, sylva_symbol name, sylva_value value);

__CPP_DECL_END

#endif // _SYLVA_RUNTIME_H_
