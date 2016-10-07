//
// Created by Ryan on 2016/10/7.
//

#include <sylva/project.h>

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

sl_project_ref sl_project_create() {
  sl_project_ref project = malloc(sizeof(sl_project));
  project->name = NULL;
  project->main_class = NULL;
  project->base_directory = NULL;
  project->source_files = sl_array_create(20);
  project->modules = sl_array_create(10);
  project->modules->value_deallocator = (sl_array_value_deallocator) &sl_module_schema_destroy;
  project->classes = sl_array_create(20);
  project->classes->value_deallocator = (sl_array_value_deallocator) &sl_class_schema_destroy;
  return project;
}

void sl_project_destroy(sl_project_ref project) {
  sl_array_destroy(project->source_files);
  sl_array_destroy(project->modules);
  sl_array_destroy(project->classes);
  free(project);
}
