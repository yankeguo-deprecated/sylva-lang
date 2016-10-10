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
#include <sylva/schema.h>

_SL_BEGIN_STD_C

typedef struct sl_project_t {
  sl_string_ref name;
  sl_string_ref main_class;
  sl_string_ref base_directory;
  sl_string_ref manifest_file_path;
  sl_string_ref build_directory;
  sl_array_ref source_files;
  sl_array_ref modules;
  sl_array_ref classes;
} sl_project;

typedef sl_project *sl_project_ref;

/**
 * create a project data
 *
 * @param base_directory
 * @return
 */
sl_project_ref sl_project_create(char *base_directory);

/**
 * load the project manifest file
 *
 * @param project
 */
void sl_project_load_manifest(sl_project_ref project);

/**
 * ensure the build directory
 *
 * @param project
 */
void sl_project_ensure_build_directory(sl_project_ref project);

/**
 * first stage, scan source files
 *
 * @param project
 */
void sl_project_scan_source_files(sl_project_ref project);

/**
 * destroy a project data pointer
 * @param project
 */
void sl_project_destroy(sl_project_ref project);

_SL_END_STD_C

#endif //SYLVA_PROJECT_H
