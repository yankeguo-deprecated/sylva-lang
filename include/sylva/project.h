//
//  project.h
//  sylva
//
//  Created by Ryan Guo<ryan@islandzero.net> on 2016/10/6.
//

#ifndef _SYLVA_PROJECT_H_
#define _SYLVA_PROJECT_H_

#include <sylva/define.h>
#include <sylva/util.h>

__BEGIN_STD_C

/**
 * sl_func_schema contains all definitions for a sylva function
 */
typedef struct sl_func_schema_t {
  /**
   * name of the function
   */
  char *name;
  /**
   * static function
   */
  bool is_static;
  /**
   * required function for module
   */
  bool is_required;
  /**
   * native function, no implementation
   */
  bool is_native;
  /**
   * impelmentation source code
   */
  char *implementation;
  /**
   * argument names, array of char*
   */
  sl_array_ref argument_names;

} sl_func_schema;

typedef sl_func_schema *sl_func_schema_ref;

/**
 * create a pointer to sl_func_schema
 *
 * @return
 */
sl_func_schema_ref sl_func_schema_create();

/**
 * destroy a pointer of sl_func_schema
 *
 * @param func_schema
 */
void sl_func_schema_destroy(sl_func_schema_ref func_schema);

/**
 * sl_member_schema contains all definitions for a member variable
 */
typedef struct sl_member_schema_t {
  /**
   * member name
   */
  char *name;
  /**
   * static member
   */
  bool is_static;
  /**
   * required member in module, do not declared
   */
  bool is_required;
  /**
   * weather this member variable is weak
   */
  bool is_weak;

} sl_member_schema;

typedef sl_member_schema *sl_member_schema_ref;

sl_member_schema_ref sl_member_schema_create();

void sl_member_schema_destroy(sl_member_schema_ref schema);

/**
 * schema for module declaration
 */
typedef struct sl_module_schema_t {
  /**
   * name of the module
   */
  char *name;
  /**
   * included modules
   */
  sl_array_ref included_module_names;
  /**
   * member variables
   */
  sl_array_ref members;
  /**
   * functions
   */
  sl_array_ref functions;
} sl_module_schema;

typedef sl_module_schema *sl_module_schema_ref;

sl_module_schema_ref sl_module_schema_create();

void sl_module_schema_destroy(sl_module_schema_ref schema);

typedef struct sl_class_schema_t {
  char *name;
} sl_class_schema;

typedef struct sl_project_t {
  char *name;
  char *main_class;
  char *base_directory;
  char **source_files;
  sl_index source_files_length;
} sl_project;

typedef sl_project *sl_project_ref;

void sl_project_init(sl_project_ref project);

__END_STD_C

#endif //SYLVA_PROJECT_H
