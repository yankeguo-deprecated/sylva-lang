//
// Created by Ryan on 2016/9/26.
//

#include "sylva/runtime.h"

#include <stdlib.h>

/**
 * calculate number of instance members of a class, including superclass
 *
 * This function will recursively iterate to superclass
 *
 * @param class the class to calculate
 * @param out output
 */
void _sylva_class_calculate_instance_members_count(sylva_class class, sylva_index *out) {
  if (class.instance_member_list != NULL) {
    *out += class.instance_member_list->length;
  }
  if (class.super != NULL) {
    _sylva_class_calculate_instance_members_count(*class.super, out);
  }
}

/**
 * initialize an allocated storage of instance variables of a object
 *
 * This function will recursively iterate to superclass, including superclass
 *
 * @param class the class used to initliaze
 * @param members the storage to iniailize
 * @param start_idx the start index to initialization, should be 0 at first
 */
void _sylva_class_init_instance_members(sylva_class class, sylva_members_ref members, sylva_index start_idx) {
  if (class.instance_member_list != NULL) {
    start_idx += sylva_members_init(members, start_idx, *class.instance_member_list);
  }
  if (class.super != NULL) {
    _sylva_class_init_instance_members(*class.super, members, start_idx);
  }
}

sylva_object_ref sylva_object_create(sylva_class_ref class) {
  sylva_object_ref object = malloc(sizeof(sylva_object));
  object->class = class;
  object->deinitializing = sylva_false;
  //  Calculate members.length
  sylva_index members_length = 0;
  _sylva_class_calculate_instance_members_count(*class, &members_length);
  //  Alloc
  object->members = sylva_members_create(members_length);
  //  Assign options and member_ids
  _sylva_class_init_instance_members(*class, object->members, 0);
  //  ref_count
  object->ref_count = 0;
  //  Iterating class tree
  return object;
}

sylva_object_ref sylva_object_retain(sylva_object_ref object) {
  object->ref_count++;
  return object;
}

sylva_object_ref sylva_object_release(sylva_object_ref object) {
  if (object->ref_count <= 1) {
    object->ref_count = 0;
    sylva_object_destroy(object);
    return NULL;
  } else {
    object->ref_count--;
    return object;
  }
}

void sylva_object_destroy(sylva_object_ref object) {
  //  prevent loop
  if (object == NULL || object->deinitializing)
    return;
  object->deinitializing = sylva_true;
  //  call custom deinitializer
  sylva_class_ref class = object->class;
  while (class != NULL) {
    if (class->deinitializor != NULL) {
      //  call custom deinitializer
      ((sylva_func) class->deinitializor)(sylva_object_value(object), sylva_args_empty);
    }
    //  iterate to superclass
    class = class->super;
  }
  //  destroy members
  sylva_members_destroy(object->members);
  //  free the object it self
  free(object);
}
