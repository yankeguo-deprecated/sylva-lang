//
// Created by Yanke Guo on 2016/9/22.
//

#ifndef _SYLVA_RUNTIME_H_
#define _SYLVA_RUNTIME_H_

#include "sylva/define.h"

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

__CPP_DECL_START


/***********************************************************************************************************************
 * Assert
 **********************************************************************************************************************/

#define sl_assert(CLAUSE, ...) if (!(CLAUSE)) { fprintf(stderr, __VA_ARGS__); abort(); }

/***********************************************************************************************************************
 * Forward Declarations
 **********************************************************************************************************************/

/*
 * Struct Declarations
 */
typedef struct sl_class_t sl_class;
typedef struct sl_object_t sl_object;
typedef struct sl_funcs_t sl_funcs;
typedef struct sl_member_defs_t sl_member_defs;
typedef struct sl_members_t sl_members;
typedef struct sl_args_t sl_args;

/*
 * Pointer Declarations
 */
typedef sl_class *sl_class_ref;
typedef sl_object *sl_object_ref;
typedef sl_funcs *sl_funcs_ref;
typedef sl_member_defs *sl_member_defs_ref;
typedef sl_members *sl_members_ref;

/***********************************************************************************************************************
 * Class & Object
 **********************************************************************************************************************/

/*
 * !!WARN!! Module in sylva-lang is a compile-time only concept
 */

/**
 * Class in sylva-lang
 *
 * for best performance, every sl_class is statically initizlied in compile result
 */
struct sl_class_t {
  /**
   * name of class
   */
  sl_symbol name;

  /**
   * pointer to superclass
   */
  sl_class_ref super;

  /**
   * static functions decalred by this class
   */
  sl_funcs_ref static_funcs;

  /**
   * instance functions declared by this class
   */
  sl_funcs_ref instance_funcs;

  /**
   * static member variables declared by this class
   */
  sl_member_defs_ref static_member_defs;

  /**
   * instance member variables declared by this class
   */
  sl_member_defs_ref instance_member_defs;

  /**
   * storage for all static member variables
   */
  sl_members_ref members;

  /**
   * deinitializer (sl_func)
   */
  void *deinitializor;
};

/**
 * Object in sylva-lang
 *
 * every object in sylva is dynamically initialized, except some special classes (String)
 */
struct sl_object_t {
  /**
   * pointer to class
   */
  sl_class_ref class;
  /**
   * storage for instance variables, including variables declared in superclass
   */
  sl_members_ref members;
  /**
   * reference counter, used for garbage collection
   */
  sl_index ref_count;
  /**
   * deinitializing
   */
  sl_boolean deinitializing;
};

/**
 * allocate memory for an object of class, including storage for member variables
 *
 * @return pointer to created object
 */
SYLVA_RUNTIME_EXTERN sl_object_ref sl_object_create(sl_class_ref class);

/**
 * increase reference counter of a object
 *
 * @param object pointer to target object
 */
SYLVA_RUNTIME_EXTERN sl_object_ref sl_object_retain(sl_object_ref object);

/**
 * decrease reference counter of a object, if reaches 0, destroy it
 *
 * @param object pointer to target object
 */
SYLVA_RUNTIME_EXTERN sl_object_ref sl_object_release(sl_object_ref object);

/**
 * destroy a object
 *
 * free all memory allocated for object, including storage for member variables
 *
 * @param object pointer to target object
 */
SYLVA_RUNTIME_EXTERN void sl_object_destroy(sl_object_ref object);

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
      sl_type_nil = 1,
  /**
   * type for sl_boolean
   */
      sl_type_boolean = 1 << 1,
  /**
   * type for sl_integer
   */
      sl_type_integer = 1 << 2,
  /**
   * type for sl_float
   */
      sl_type_float = 1 << 3,
  /**
   * sum all numeric types
   */
      sl_type_numeric = sl_type_integer | sl_type_float,
  /**
   * sum all primitive types
   */
      sl_type_primitive = sl_type_nil | sl_type_boolean | sl_type_integer | sl_type_float,
  /**
   * type for sl_object_ref
   */
      sl_type_object = 1 << 4,
  /**
   * type for sl_class_ref
   */
      sl_type_class = 1 << 5,
  /**
   * type for wild C pointer, sylva-runtime will never use this type, reserved for native bindings
   */
      sl_type_pointer = 1 << 6,

} sl_value_type;

/**
 * Value for sylva-lang, it's a mixin, support all types used in sylva-lang
 */
typedef struct {
  /**
   * type for this sl_value
   */
  sl_value_type type;

  union {
    /**
     * value as sl_integer
     */
    sl_integer integer_value;
    /**
     * value for sl_float
     */
    sl_float float_value;
    /**
     * value for sl_boolean
     */
    sl_boolean boolean_value;
    /**
     * value for sl_class pointer
     */
    sl_class_ref class_value;
    /**
     * value for sl_object pointer
     */
    sl_object_ref object_value;
    /**
     * value for C pointer, will be same as class_value and object_value
     */
    void *pointer_value;
  };
} sl_value;

/**
 * pointer to Value
 */
typedef sl_value *sl_value_ref;

/**
 * bare sl_value initializer macros, used for statically init
 */
#define sl_bare_pointer_value(X) {.type = sl_type_pointer, .pointer_value = (X)}
#define sl_bare_nil_value {.type = sl_type_nil, .integer_value = 0}
#define sl_bare_boolean_value(X) {.type = sl_type_boolean, .boolean_value = (X)}
#define sl_bare_true_value {.type = sl_type_boolean, .boolean_value = sl_true}
#define sl_bare_false_value {.type = sl_type_boolean, .boolean_value = sl_false}
#define sl_bare_integer_value(X) {.type = sl_type_integer, .integer_value = (X)}
#define sl_bare_float_value(X) {.type = sl_type_float, .float_value = (X)}
#define sl_bare_object_value(X) {.type = sl_type_object, .object_value = (X)}
#define sl_bare_class_value(X) {.type = sl_type_class, .class_value = (X)}

/**
 * sl_value initializer macros
 */
#define sl_pointer_value(X)  ((sl_value) sl_bare_pointer_value(X))
#define sl_nil_value ((sl_value) sl_bare_nil_value)
#define sl_boolean_value(X) ((sl_value) sl_bare_boolean_value(X))
#define sl_true_value sl_boolean_value(sl_true)
#define sl_false_value sl_boolean_value(sl_false)
#define sl_integer_value(X) ((sl_value) sl_bare_integer_value(X))
#define sl_float_value(X) ((sl_value) sl_bare_float_value(X))
#define sl_object_value(X) ((sl_value) sl_bare_object_value(X))
#define sl_class_value(X) ((sl_value) sl_bare_class_value(X))

/**
 * turns any sl_value to a resonable boolean value
 *
 * @param value sl_value
 *
 * @return sl_true or sl_false
 */
SYLVA_RUNTIME_EXTERN sl_boolean sl_to_boolean(sl_value value);

/**
 * turns any sl_value to a resonable integer value
 *
 * @param value sl_value
 *
 * @return sl_value with integer value type
 */
SYLVA_RUNTIME_EXTERN sl_integer sl_to_integer(sl_value value);

/**
 * turns any sl_value to a resonable integer value
 *
 * @param value sl_value
 *
 * @return sl_value with integer value type
 */
SYLVA_RUNTIME_EXTERN sl_float sl_to_float(sl_value value);

/**
 * turns to number values
 *
 * @param value pointer to value to convert
 */
SYLVA_RUNTIME_EXTERN void sl_trans_to_numeric(sl_value_ref value);

/**
 * turns to float
 *
 * @param value pointer to value to convert
 */
SYLVA_RUNTIME_EXTERN void sl_trans_to_float(sl_value_ref value);

/**
 * turns to integer
 *
 * @param value pointer to value to convert
 */
SYLVA_RUNTIME_EXTERN void sl_trans_to_integer(sl_value_ref value);

/**
 * turns to boolean
 *
 * @param value pointer to value to convert
 */
SYLVA_RUNTIME_EXTERN void sl_trans_to_boolean(sl_value_ref value);

/***********************************************************************************************************************
 * Function Arguments
 **********************************************************************************************************************/

/**
 * Arguments for function invocation
 */
struct sl_args_t {
  /**
   * number of arguments
   */
  sl_index length;
  /**
   * storage for argument values
   */
  sl_value *values;
};

/**
 * pointer to Arguments
 */
typedef sl_args *sl_args_ref;

/**
 * initializer macro for sl_args
 */
#define sl_args_make(C, ...) ((sl_args){ .length = C, .values = (sl_value[]){ __VA_ARGS__ }})
#define sl_args_empty ((sl_args){ .length = 0, .values = NULL })

/**
 * create a sl_args with length
 *
 * @param length number of arguments
 * @param list arguments stored in va_list
 *
 * @return sl_args
 */
SYLVA_RUNTIME_EXTERN sl_args sl_args_create(sl_index length, ...);
SYLVA_RUNTIME_EXTERN sl_args sl_args_v_create(sl_index length, va_list list);

/**
 * get a value at index from sl_args
 *
 * @param args sl_args
 * @param index index of value to get
 *
 * @return sl_value at index
 */
SYLVA_RUNTIME_EXTERN sl_value sl_args_get(sl_args args, sl_index index);

/**
 * destroy a previousely created sl_args
 *
 * @param sl_args to destroy
 */
SYLVA_RUNTIME_EXTERN void sl_args_destroy(sl_args args);

/***********************************************************************************************************************
 * Function
 **********************************************************************************************************************/

/**
 * sl_func is the general function type in sylva-lang, including instance functions and static functions
 *
 * !!WARN!! context may be those of subclasses
 *
 * @param context for instance functions this is the object, for class functions this is the class
 * @param args arguments passed in, will be automatically destroyed
 */
typedef sl_value (*sl_imp)(sl_value context, sl_args args);

/**
 * invoke a sl_func, with context and variable arguments
 *
 * @param func function to invoke
 * @param context context for function, basically sl_object or sl_class
 * @param length number of arguments
 * @param list va_list
 *
 * @return result of function invocation
 */
SYLVA_RUNTIME_EXTERN sl_value sl_imp_call(sl_imp imp, sl_value context, sl_index length, ...);
SYLVA_RUNTIME_EXTERN sl_value sl_imp_v_call(sl_imp imp, sl_value context, sl_index length, va_list list);

/***********************************************************************************************************************
 * Function Storage
 **********************************************************************************************************************/

/**
 * function storage entry
 */
typedef struct sl_func_t {
  /**
   * function name
   */
  char *name;

  /**
   * function implementation
   */
  sl_imp imp;

} sl_func;

#define sl_func_item(NAME, IMP) { .name = NAME, .imp = IMP }
#define sl_func_make(NAME, IMP) ((sl_func) sl_func_item(NAME, IMP))

/**
 * pointer type for sl_funcs_entry
 */
typedef sl_func *sl_func_ref;

/**
 * sl_funcs stores a sl_symbol to sl_func map
 *
 * !!WARN!! in an ideal situation, sl_funcs should be initialized literally
 */
struct sl_funcs_t {
  /**
   * number of functions
   */
  sl_index length;

  /**
   * entries
   */
  sl_func_ref entries;
};

#define sl_funcs_make(L, ...) ((sl_funcs) { .length = L, .entries = (sl_func[]) { __VA_ARGS__ }})

/**
 * find a pointer of function from sl_funcs by function id
 *
 * @param list sl_funcs to find from
 * @param func_id function id to find
 *
 * @return pointer to function
 */
SYLVA_RUNTIME_EXTERN sl_imp sl_funcs_get(sl_funcs list, sl_symbol name);

/**
 * set a sl_func to a function id, a NULL sl_func will not remove function id from sl_funcs
 *
 * @param list sl_funcs to set
 * @param func_id function id to set
 * @param func pointer to function to set
 *
 * @return NULL if out of length, func if succeeded
 */
SYLVA_RUNTIME_EXTERN sl_imp sl_funcs_set(sl_funcs list, sl_symbol name, sl_imp func);

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
      sl_member_normal = 0,
  /**
   * weak variable, will retain/release while setting
   */
      sl_member_weak = 1 << 0,

} sl_member_option;

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
  sl_member_option option;
} sl_member_def;

typedef sl_member_def *sl_member_def_ref;

#define sl_member_def_item(NAME, OPTION) { .name = NAME, .option = OPTION }
#define sl_member_def_make(NAME, OPTION) ((sl_member_def) sl_member_def_item(NAME, OPTION))

/**
 * entry for members storage
 */
typedef struct sl_member_t {
  /**
   * name
   */
  char *name;
  /**
   * option
   */
  sl_member_option option;
  /**
   * value
   */
  sl_value value;
} sl_member;

#define sl_member_item(NAME, OPTION) { .name = NAME, .option = OPTION, .value = sl_bare_nil_value }
#define sl_member_make(NAME, OPTION) ((sl_member) sl_member_item(NAME, OPTION))

/**
 * pointer
 */
typedef sl_member *sl_members_entry_ref;

/**
 * Member Viarable Declarations
 *
 * !!WARN!! in ideal situation, member viarable is declared statically
 */
struct sl_member_defs_t {
  /**
   * number of members
   */
  sl_index length;
  /**
   * member ids
   */
  sl_member_def_ref entries;
};

#define sl_member_defs_make(L, ...) ((sl_member_defs) { .length = L, .entries = (sl_member_def[]){ __VA_ARGS__ }})

/**
 * Member Storage
 *
 * !!WARN!! in ideal situation, class
 */
struct sl_members_t {
  /**
   * number of members
   */
  sl_index length;
  /**
   * entries
   */
  sl_members_entry_ref entries;
};

#define sl_members_make(L, ...) ((sl_members) { .length = L, .entries = (sl_member[]) { __VA_ARGS__ }})

/**
 * allocate memory for member storage with length provided
 *
 * @param length number of members
 *
 * @return pointer of allocated member storage
 */
SYLVA_RUNTIME_EXTERN sl_members_ref sl_members_create(sl_index length);

/**
 * init a members storage with members declared in a member list
 *
 * @param members pointer to member storage
 * @param start_idx from which index the member storage should init
 * @param member_list the member list should init from
 *
 * @return number of members inited
 */
SYLVA_RUNTIME_EXTERN sl_index sl_members_init(sl_members_ref members, sl_index start_idx, sl_member_defs defs);

/**
 * get a value from member storage with member id
 *
 * @param members pointer to member storage
 * @param member_id member id to get
 *
 * @return member value, sl_value_nil if not found
 */
SYLVA_RUNTIME_EXTERN sl_value sl_members_get(sl_members members, sl_symbol name);

/**
 * set a value to member storage with member id
 *
 * !!WARN!! this will invoke retain/release automatically
 *
 * @param members pointer to member storage
 * @param member_id member id to set
 * @param value value to set, will retain if this is not a weak member
 *
 * @param sl_true if set, sl_false if member id not found
 */
SYLVA_RUNTIME_EXTERN sl_boolean sl_members_set(sl_members members, sl_symbol name, sl_value value);

/**
 * destroy a member storage, will free everything and release all members
 */
SYLVA_RUNTIME_EXTERN void sl_members_destroy(sl_members_ref members);

/***********************************************************************************************************************
 * Object-Oriented
 **********************************************************************************************************************/

/**
 * Get the corresponding class value from a value, also resolves virtual class for primitive values
 *
 * @param value incoming value
 * @return class associated
 */
SYLVA_RUNTIME_EXTERN sl_class_ref sl_get_class(sl_value value);

/**
 * resolve a function id to instance function from a Class, including superclass
 *
 * @param class Class to resolve from
 * @param func_id function id to resolve
 *
 * @return pointer to function if resolved, NULL or not
 */
SYLVA_RUNTIME_EXTERN sl_imp sl_class_instance_func_resolve(sl_class class, sl_symbol name);

/**
 * resolve a function id to static function from a Class, including superclass
 *
 * @param class Class to resolve from
 * @param func_id function id to resolve
 *
 * @return pointer to function if resolved, NULL or not
 */
SYLVA_RUNTIME_EXTERN sl_imp sl_class_static_func_resolve(sl_class class, sl_symbol name);

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
SYLVA_RUNTIME_EXTERN sl_imp sl_func_resolve(sl_value context, sl_symbol name);

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
SYLVA_RUNTIME_EXTERN sl_imp sl_func_resolve_super(sl_value context, sl_class_ref class, sl_symbol name);

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
SYLVA_RUNTIME_EXTERN sl_value sl_call(sl_value context, sl_symbol name, sl_index length, ...);
SYLVA_RUNTIME_EXTERN sl_value sl_v_call(sl_value context, sl_symbol name, sl_index length, va_list list);

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
SYLVA_RUNTIME_EXTERN sl_value sl_call_super(sl_value context,
                                            sl_class_ref class,
                                            sl_symbol name,
                                            sl_index length,
                                            ...);
SYLVA_RUNTIME_EXTERN sl_value sl_v_call_super(sl_value context,
                                              sl_class_ref class,
                                              sl_symbol name,
                                              sl_index length,
                                              va_list list);

/**
 * create a instance of a class and initialize with specifed initializer
 *
 * @param class class to intialize
 * @param func_id function id to initialize
 * @param length number of arguments
 * @param list arguments in va_list
 */
SYLVA_RUNTIME_EXTERN sl_value sl_create(sl_class_ref class, sl_symbol name, sl_index length, ...);

SYLVA_RUNTIME_EXTERN sl_value sl_v_create(sl_class_ref class, sl_symbol name, sl_index length, va_list list);

/**
 * invoke sl_object_retain if value is an object
 *
 * do nothing if value is not an object
 *
 * @param value sl_value to ratain
 */
SYLVA_RUNTIME_EXTERN void sl_retain(sl_value_ref value);

/**
 * invoke sl_object_release if value is an object
 *
 * do nothing if value is not a object
 *
 * @param value sl_value to ratain
 */
SYLVA_RUNTIME_EXTERN void sl_release(sl_value_ref value);

/**
 * get member from object, class and value(object/class type)
 *
 * @see sl_members_get
 */
SYLVA_RUNTIME_EXTERN sl_value sl_object_members_get(sl_object object, sl_symbol name);
SYLVA_RUNTIME_EXTERN sl_value sl_class_members_get(sl_class class, sl_symbol name);
SYLVA_RUNTIME_EXTERN sl_value sl_get(sl_value value, sl_symbol name);
SYLVA_RUNTIME_EXTERN sl_value sl_static_get(sl_value value, sl_symbol name);

/**
 * set member to object, class and value(object/class type)
 *
 * @see sl_members_set
 */
SYLVA_RUNTIME_EXTERN sl_boolean sl_object_members_set(sl_object object, sl_symbol name, sl_value value);
SYLVA_RUNTIME_EXTERN sl_boolean sl_class_members_set(sl_class class, sl_symbol name, sl_value value);
SYLVA_RUNTIME_EXTERN sl_boolean sl_set(sl_value target_value, sl_symbol name, sl_value value);
SYLVA_RUNTIME_EXTERN sl_boolean sl_static_set(sl_value target_value, sl_symbol name, sl_value value);

/***********************************************************************************************************************
 * Pre-declaration for Foundation
 **********************************************************************************************************************/

sl_class SYLVA_Number;
sl_class SYLVA_Object;
sl_class SYLVA_String;

__CPP_DECL_END

#endif // _SYLVA_RUNTIME_H_
