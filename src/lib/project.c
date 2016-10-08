// project.c
// sylva
//
// Created by Ryan Guo<ryan@islandzero.net> on 2016/10/7.
//

#include <sylva/project.h>
#include <sylva/util.h>
#include <sylva/parser.h>

#include <yaml.h>
#include <sys/stat.h>

sl_project_ref sl_project_create(char *base_directory) {
  sl_project_ref project = malloc(sizeof(sl_project));
  project->name = NULL;
  project->main_class = NULL;

  //  record base_directory, append tailing '/' if not present
  project->base_directory = sl_string_create(base_directory);
  if (sl_string_get_last(project->base_directory) != '/') {
    sl_string_append(project->base_directory, "/");
  }
  //  manifest path
  project->manifest_file_path = sl_string_create(project->base_directory->string);
  sl_string_append(project->manifest_file_path, MANIFEST_FILE);
  //  build directory
  project->build_directory = sl_string_create(project->base_directory->string);
  sl_string_append(project->build_directory, BUILD_DIRECTORY);
  sl_string_append(project->build_directory, "/");

  project->source_files = sl_array_create(20);
  project->source_files->value_deallocator = (sl_array_value_deallocator) &sl_string_destroy;

  project->modules = sl_array_create(10);
  project->modules->value_deallocator = (sl_array_value_deallocator) &sl_module_schema_destroy;

  project->classes = sl_array_create(20);
  project->classes->value_deallocator = (sl_array_value_deallocator) &sl_class_schema_destroy;

  return project;
}

void sl_project_load_manifest(sl_project_ref project) {
  FILE *file = fopen(project->manifest_file_path->string, "r");
  if (file == NULL) {
    perror("fopen()");
    abort();
  }
  //  parse manifest
  yaml_parser_t parser;
  yaml_document_t document;
  yaml_node_t *root_node;

  yaml_parser_initialize(&parser);
  yaml_parser_set_encoding(&parser, YAML_UTF8_ENCODING);
  yaml_parser_set_input_file(&parser, file);
  yaml_parser_load(&parser, &document);

  sl_assert(parser.error == YAML_NO_ERROR,
            "Failed to parse %s at line %ld, column %ld, %s\n",
            project->manifest_file_path->string,
            parser.problem_mark.line,
            parser.problem_mark.column,
            parser.problem);

  root_node = yaml_document_get_root_node(&document);

  yaml_node_pair_t *node = NULL;

  for (node = root_node->data.mapping.pairs.start; node < root_node->data.mapping.pairs.top; node++) {
    yaml_node_t *name = yaml_document_get_node(&document, node->key);
    yaml_node_t *value = yaml_document_get_node(&document, node->value);
    sl_assert(name->type == YAML_SCALAR_NODE, "expecting yaml key to be a scalar");
    //  name
    if (strcmp((const char *) name->data.scalar.value, "name") == 0) {
      sl_assert(value->type == YAML_SCALAR_NODE, "expecting 'name' to be a string");
      project->name = sl_string_create((char *) value->data.scalar.value);
    }
    //  source files
    if (strcmp((const char *) name->data.scalar.value, "source_files") == 0) {
      sl_assert(value->type == YAML_SEQUENCE_NODE, "expecting 'source_files' to be a array of string");
      for (yaml_node_item_t *item = value->data.sequence.items.start; item < value->data.sequence.items.top; item++) {
        yaml_node_t *file_node = yaml_document_get_node(&document, *item);
        sl_assert(file_node->type == YAML_SCALAR_NODE, "expecting 'source_files' to be a array of string");
        sl_string_ref file_path = sl_string_create((char *) file_node->data.scalar.value);
        sl_array_append(project->source_files, file_path);
      }
    }
    //  main class
    if (strcmp((const char *) name->data.scalar.value, "main_class") == 0) {
      sl_assert(value->type == YAML_SCALAR_NODE, "expecting 'main_class' to be a string");
      project->main_class = sl_string_create((char *) value->data.scalar.value);
    }
  }

  printf("Project: %s\n", project->name->string);
  printf("Source Files:\n");
  for (sl_index i = 0; i < project->source_files->count; ++i) {
    sl_string_ref file_name = project->source_files->values[i];
    printf("  %s\n", file_name->string);
  }

  yaml_document_delete(&document);
  yaml_parser_delete(&parser);
  fclose(file);
}

void sl_project_ensure_build_directory(sl_project_ref project) {
  mkdir(project->build_directory->string, ACCESSPERMS);
}

void sl_project_scan_source_files(sl_project_ref project) {
  for (sl_index i = 0; i < project->source_files->count; i++) {
    sl_string_ref file_name = project->source_files->values[i];
    sl_string_ref file_path = sl_string_create(project->base_directory->string);
    sl_string_append(file_path, file_name->string);
    sl_parser_ref parser = sl_parser_create(file_path->string);
    sl_parser_scan_to_project(parser, project);
    sl_parser_destroy(parser);
    sl_string_destroy(file_path);
  }
}

void sl_project_destroy(sl_project_ref project) {
  sl_string_destroy(project->name);
  sl_string_destroy(project->base_directory);
  sl_string_destroy(project->manifest_file_path);
  sl_string_destroy(project->build_directory);
  sl_string_destroy(project->main_class);
  sl_array_destroy(project->source_files);
  sl_array_destroy(project->modules);
  sl_array_destroy(project->classes);
  free(project);
}
