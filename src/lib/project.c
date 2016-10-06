//
// Created by Ryan on 2016/10/7.
//

#include <sylva/project.h>

sl_func_schema_ref sl_func_schema_create() {
  sl_func_schema_ref schema = malloc(sizeof(sl_func_schema));
  schema->name = NULL;
  schema->class_name = NULL;
  schema->is_static = false;
  schema->is_module = false;
  schema->is_required = false;
  schema->is_native = false;
  schema->implementation = NULL;
  schema->argument_names = sl_array_create(5);
  return schema;
}

void sl_func_schema_destroy(sl_func_schema_ref func_schema) {
  sl_array_destroy(func_schema->argument_names, false);
  free(func_schema);
}
