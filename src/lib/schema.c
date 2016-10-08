//
// schema.c
// sylva
//
// Created by Ryan Guo<ryan@islandzero.net> on 2016/10/8.
//

#include <sylva/schema.h>

sl_func_schema_ref sl_func_schema_create() {
  sl_func_schema_ref schema = malloc(sizeof(sl_func_schema));
  schema->name = NULL;
  schema->is_static = false;
  schema->is_required = false;
  schema->is_native = false;
  schema->implementation = NULL;
  schema->argument_names = sl_array_create(5);
  return schema;
}

void sl_func_schema_destroy(sl_func_schema_ref func_schema) {
  sl_array_destroy(func_schema->argument_names);
  free(func_schema);
}

sl_member_schema_ref sl_member_schema_create() {
  sl_member_schema_ref schema = malloc(sizeof(sl_member_schema));
  schema->name = NULL;
  schema->is_static = false;
  schema->is_weak = false;
  schema->is_required = false;
  return schema;
}

void sl_member_schema_destroy(sl_member_schema_ref schema) {
  free(schema);
}

sl_module_schema_ref sl_module_schema_create() {
  sl_module_schema_ref schema = malloc(sizeof(sl_module_schema));
  schema->name = NULL;
  schema->included_module_names = sl_array_create(1);
  schema->members = sl_array_create(5);
  schema->members->value_deallocator = (sl_array_value_deallocator) &sl_member_schema_destroy;
  schema->functions = sl_array_create(5);
  schema->functions->value_deallocator = (sl_array_value_deallocator) &sl_func_schema_destroy;
  return schema;
}

void sl_module_schema_destroy(sl_module_schema_ref schema) {
  sl_array_destroy(schema->included_module_names);
  sl_array_destroy(schema->members);
  sl_array_destroy(schema->functions);
  free(schema);
}

sl_class_schema_ref sl_class_schema_create() {
  sl_class_schema_ref schema = malloc(sizeof(sl_class_schema));
  schema->name = NULL;
  schema->super_class_name = NULL;
  schema->included_module_names = sl_array_create(1);
  schema->members = sl_array_create(5);
  schema->members->value_deallocator = (sl_array_value_deallocator) &sl_member_schema_destroy;
  schema->functions = sl_array_create(5);
  schema->functions->value_deallocator = (sl_array_value_deallocator) &sl_func_schema_destroy;
  return schema;
}

void sl_class_schema_destroy(sl_class_schema_ref schema) {
  sl_array_destroy(schema->included_module_names);
  sl_array_destroy(schema->members);
  sl_array_destroy(schema->functions);
  free(schema);
}
