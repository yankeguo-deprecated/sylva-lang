//
// Created by Yanke Guo on 2016/9/22.
//

#ifndef _SYLVA_RUNTIME_H_
#define _SYLVA_RUNTIME_H_

#include "sylva/define.h"

__BEGIN_STD_C

/***********************************************************************************************************************
 * Sylva Runtime Naming
 **********************************************************************************************************************/

#define sl_class_name(CLASS) SYLVA_C_##CLASS
#define sl_class_decl(CLASS) sl_class sl_class_name(CLASS)

#define sl_func_name_class_instance(CLASS, NAME) SYLVA_C_##CLASS##_I_##NAME
#define sl_func_name_class_static(CLASS, NAME) SYLVA_C_##CLASS##_S_##NAME

#define sl_func_decl_class_instance(CLASS, NAME) sl_value sl_func_name_class_instance(CLASS, NAME)(__unused sl_value self, __unused sl_args arguments)
#define sl_func_decl_class_static(CLASS, NAME) sl_value sl_func_name_class_static(CLASS, NAME)(__unused sl_value self, __unused sl_args arguments)

#define sl_func_name_module_instance(MODULE, NAME) SYLVA_M_##MODULE##_I_##NAME
#define sl_func_name_module_static(MODULE, NAME) SYLVA_M_##MODULE##_S_##NAME

#define sl_func_decl_module_instance(MODULE, NAME) sl_value sl_func_name_module_instance(MODULE, NAME)(__unused sl_value self, __unused sl_args arguments)
#define sl_func_decl_module_static(MODULE, NAME) sl_value sl_func_name_module_static(MODULE, NAME)(__unused sl_value self, __unused sl_args arguments)

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
};

#define sl_class_decl_start(CLASS) \
  sl_class_decl(CLASS) = {\
    .name = #CLASS,

#define sl_class_decl_super(SUPER) .super = &sl_class_name(SUPER),
#define sl_class_decl_super_null .super = NULL,

#define sl_class_decl_static_funcs_null .static_funcs = NULL,
#define sl_class_decl_static_funcs_start(LENGTH)\
  .static_funcs = &(sl_funcs) {\
    .length = LENGTH,\
    .entries = (sl_func[]) {
#define sl_class_decl_static_class_func(CLASS, NAME) sl_func_item(#NAME, &sl_func_name_class_static(CLASS, NAME)),
#define sl_class_decl_static_module_func(MODULE, NAME) sl_func_item(#NAME, &sl_func_name_module_static(MODULE, NAME)),
#define sl_class_decl_static_funcs_end }},

#define sl_class_decl_instance_funcs_null .instance_funcs = NULL,
#define sl_class_decl_instance_funcs_start(LENGTH)\
  .instance_funcs = &(sl_funcs) {\
    .length = LENGTH,\
    .entries = (sl_func[]) {
#define sl_class_decl_instance_class_func(CLASS, NAME) sl_func_item(#NAME, &sl_func_name_class_instance(CLASS, NAME)),
#define sl_class_decl_instance_module_func(MODULE, NAME) sl_func_item(#NAME, &sl_func_name_module_instance(MODULE, NAME)),
#define sl_class_decl_instance_funcs_end }},

#define sl_class_decl_member_def(NAME, OPTION) sl_member_def_item(#NAME, sl_member_##OPTION),

#define sl_class_decl_static_member_defs_null .static_member_defs = NULL,
#define sl_class_decl_static_member_defs_start(LENGTH)\
  .static_member_defs = &(sl_member_defs) {\
    .length = LENGTH,\
    .entries = (sl_member_def[]){
#define sl_class_decl_static_member_defs_end }},

#define sl_class_decl_instance_member_defs_null .instance_member_defs = NULL,
#define sl_class_decl_instance_member_defs_start(LENGTH)\
  .instance_member_defs = &(sl_member_defs) {\
    .length = LENGTH,\
    .entries = (sl_member_def[]){
#define sl_class_decl_instance_member_defs_end }},

#define sl_class_decl_members_null .members = NULL,
#define sl_class_decl_members_start(LENGTH)\
  .members = &(sl_members) {\
    .length = LENGTH,\
    .entries = (sl_member[]){
#define sl_class_decl_member(NAME, OPTION) sl_member_item(#NAME, sl_member_##OPTION),
#define sl_class_decl_members_end }},

#define sl_class_decl_end };

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
sl_object_ref sl_object_create(sl_class_ref class);

/**
 * increase reference counter of a object
 *
 * @param object pointer to target object
 */
sl_object_ref sl_object_retain(sl_object_ref object);

/**
 * decrease reference counter of a object, if reaches 0, destroy it
 *
 * @param object pointer to target object
 */
sl_object_ref sl_object_release(sl_object_ref object);

/**
 * destroy a object
 *
 * free all memory allocated for object, including storage for member variables
 *
 * @param object pointer to target object
 */
void sl_object_destroy(sl_object_ref object);

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
    sl_integer as_integer;
    /**
     * value for sl_float
     */
    sl_float as_float;
    /**
     * value for sl_boolean
     */
    sl_boolean as_boolean;
    /**
     * value for sl_class pointer
     */
    sl_class_ref as_class;
    /**
     * value for sl_object pointer
     */
    sl_object_ref as_object;
    /**
     * value for C pointer, will be same as class_value and object_value
     */
    void *as_pointer;
  } value;

} sl_value;

/**
 * pointer to Value
 */
typedef sl_value *sl_value_ref;

/**
 * bare sl_value initializer macros, used for statically init
 */
#define sl_bare_pointer_value(X) {.type = sl_type_pointer, .value.as_pointer = (void *)(X)}
#define sl_bare_nil_value {.type = sl_type_nil, .value.as_integer = 0}
#define sl_bare_boolean_value(X) {.type = sl_type_boolean, .value.as_boolean = (X)}
#define sl_bare_true_value {.type = sl_type_boolean, .value.as_boolean = sl_true}
#define sl_bare_false_value {.type = sl_type_boolean, .value.as_boolean = sl_false}
#define sl_bare_integer_value(X) {.type = sl_type_integer, .value.as_integer = (X)}
#define sl_bare_float_value(X) {.type = sl_type_float, .value.as_float = (X)}
#define sl_bare_object_value(X) {.type = sl_type_object, .value.as_object = (X)}
#define sl_bare_class_value(X) {.type = sl_type_class, .value.as_class = (X)}

/**
 * sl_value initializer macros
 */
#define sl_pointer_value(X) ((sl_value) sl_bare_pointer_value(X))
#define sl_nil_value ((sl_value) sl_bare_nil_value)
#define sl_boolean_value(X) ((sl_value) sl_bare_boolean_value(X))
#define sl_true_value sl_boolean_value(sl_true)
#define sl_false_value sl_boolean_value(sl_false)
#define sl_integer_value(X) ((sl_value) sl_bare_integer_value(X))
#define sl_float_value(X) ((sl_value) sl_bare_float_value(X))
#define sl_object_value(X) ((sl_value) sl_bare_object_value(X))
#define sl_class_value(X) ((sl_value) sl_bare_class_value(X))
#define sl_class_rel(CLASS) sl_class_value(&sl_class_name(CLASS))

/**
 * turns any sl_value to a resonable boolean value
 *
 * @param value sl_value
 *
 * @return sl_true or sl_false
 */
sl_boolean sl_to_boolean(sl_value value);

/**
 * turns any sl_value to a resonable integer value
 *
 * @param value sl_value
 *
 * @return sl_value with integer value type
 */
sl_integer sl_to_integer(sl_value value);

/**
 * turns any sl_value to a resonable integer value
 *
 * @param value sl_value
 *
 * @return sl_value with integer value type
 */
sl_float sl_to_float(sl_value value);

/**
 * turns to number values
 *
 * @param value pointer to value to convert
 */
void sl_trans_to_numeric(sl_value_ref value);

/**
 * turns to float
 *
 * @param value pointer to value to convert
 */
void sl_trans_to_float(sl_value_ref value);

/**
 * turns to integer
 *
 * @param value pointer to value to convert
 */
void sl_trans_to_integer(sl_value_ref value);

/**
 * turns to boolean
 *
 * @param value pointer to value to convert
 */
void sl_trans_to_boolean(sl_value_ref value);

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
sl_args sl_args_create(sl_index length, ...);
sl_args sl_args_v_create(sl_index length, va_list list);

/**
 * get a value at index from sl_args
 *
 * @param args sl_args
 * @param index index of value to get
 *
 * @return sl_value at index
 */
sl_value sl_args_get(sl_args args, sl_index index);

/**
 * destroy a previousely created sl_args
 *
 * @param sl_args to destroy
 */
void sl_args_destroy(sl_args args);

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
sl_value sl_imp_call(sl_imp imp, sl_value context, sl_index length, ...);
sl_value sl_imp_v_call(sl_imp imp, sl_value context, sl_index length, va_list list);

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
sl_imp sl_funcs_get(sl_funcs list, sl_symbol name);

/**
 * set a sl_func to a function id, a NULL sl_func will not remove function id from sl_funcs
 *
 * @param list sl_funcs to set
 * @param func_id function id to set
 * @param func pointer to function to set
 *
 * @return NULL if out of length, func if succeeded
 */
sl_imp sl_funcs_set(sl_funcs list, sl_symbol name, sl_imp func);

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
sl_members_ref sl_members_create(sl_index length);

/**
 * init a members storage with members declared in a member list
 *
 * @param members pointer to member storage
 * @param start_idx from which index the member storage should init
 * @param member_list the member list should init from
 *
 * @return number of members inited
 */
sl_index sl_members_init(sl_members_ref members, sl_index start_idx, sl_member_defs defs);

/**
 * get a value from member storage with member id
 *
 * @param members pointer to member storage
 * @param member_id member id to get
 *
 * @return member value, sl_value_nil if not found
 */
sl_value sl_members_get(sl_members members, sl_symbol name);

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
sl_boolean sl_members_set(sl_members members, sl_symbol name, sl_value value);

/**
 * destroy a member storage, will free everything and release all members
 */
void sl_members_destroy(sl_members_ref members);

/***********************************************************************************************************************
 * Object-Oriented
 **********************************************************************************************************************/

/**
 * Get the corresponding class value from a value, also resolves virtual class for primitive values
 *
 * @param value incoming value
 * @return class associated
 */
sl_class_ref sl_get_class(sl_value value);

/**
 * resolve a function id to instance function from a Class, including superclass
 *
 * @param class Class to resolve from
 * @param func_id function id to resolve
 *
 * @return pointer to function if resolved, NULL or not
 */
sl_imp sl_class_instance_func_resolve(sl_class class, sl_symbol name);

/**
 * resolve a function id to static function from a Class, including superclass
 *
 * @param class Class to resolve from
 * @param func_id function id to resolve
 *
 * @return pointer to function if resolved, NULL or not
 */
sl_imp sl_class_static_func_resolve(sl_class class, sl_symbol name);

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
sl_imp sl_func_resolve(sl_value context, sl_symbol name);

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
sl_imp sl_func_resolve_super(sl_value context, sl_class_ref class, sl_symbol name);

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
sl_value sl_call(sl_value context, sl_symbol name, sl_index length, ...);
sl_value sl_v_call(sl_value context, sl_symbol name, sl_index length, va_list list);

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
sl_value sl_call_super(sl_value context, sl_class_ref class, sl_symbol name, sl_index length, ...);
sl_value sl_v_call_super(sl_value context, sl_class_ref class, sl_symbol name, sl_index length, va_list list);

/**
 * create a instance of a class and initialize with specifed initializer
 *
 * @param class class to intialize
 * @param func_id function id to initialize
 * @param length number of arguments
 * @param list arguments in va_list
 */
sl_value sl_create(sl_class_ref class, sl_symbol name, sl_index length, ...);
sl_value sl_v_create(sl_class_ref class, sl_symbol name, sl_index length, va_list list);

/**
 * invoke sl_object_retain if value is an object
 *
 * do nothing if value is not an object
 *
 * @param value sl_value to ratain
 */
void sl_retain(sl_value_ref value);

/**
 * invoke sl_object_release if value is an object
 *
 * do nothing if value is not a object
 *
 * @param value sl_value to ratain
 */
void sl_release(sl_value_ref value);

/**
 * get member from object, class and value(object/class type)
 *
 * @see sl_members_get
 */
sl_value sl_object_members_get(sl_object object, sl_symbol name);
sl_value sl_class_members_get(sl_class class, sl_symbol name);
sl_value sl_get(sl_value value, sl_symbol name);
sl_value sl_static_get(sl_value value, sl_symbol name);

/**
 * set member to object, class and value(object/class type)
 *
 * @see sl_members_set
 */
sl_boolean sl_object_members_set(sl_object object, sl_symbol name, sl_value value);
sl_boolean sl_class_members_set(sl_class class, sl_symbol name, sl_value value);
sl_boolean sl_set(sl_value target_value, sl_symbol name, sl_value value);
sl_boolean sl_static_set(sl_value target_value, sl_symbol name, sl_value value);

/***********************************************************************************************************************
 * Pre-declaration for Foundation
 **********************************************************************************************************************/

sl_class_decl(Number);
sl_class_decl(Object);
sl_class_decl(String);
sl_class_decl(Hash);
sl_class_decl(Array);

__END_STD_C

#endif // _SYLVA_RUNTIME_H_
