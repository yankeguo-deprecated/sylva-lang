//
// Created by Ryan on 2016/9/26.
//

#define __SYLVA_RUNTIME_SOURCE__
#include "sylva/runtime.h"

/**
 * calculate number of instance members of a class, including superclass
 *
 * This function will recursively iterate to superclass
 *
 * @param class the class to calculate
 * @param out output
 */
void _sl_class_calculate_instance_members_count(sl_class class, sl_index *out) {
  if (class.instance_member_defs != NULL) {
    *out += class.instance_member_defs->length;
  }
  if (class.super != NULL) {
    _sl_class_calculate_instance_members_count(*class.super, out);
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
void _sl_class_init_instance_members(sl_class class, sl_members_ref members, sl_index start_idx) {
  if (class.instance_member_defs != NULL) {
    start_idx += sl_members_init(members, start_idx, *class.instance_member_defs);
  }
  if (class.super != NULL) {
    _sl_class_init_instance_members(*class.super, members, start_idx);
  }
}

sl_object_ref sl_object_create(sl_class_ref class) {
  sl_object_ref object = malloc(sizeof(sl_object));
  object->class = class;
  object->deinitializing = sl_false;
  //  Calculate members.length
  sl_index members_length = 0;
  _sl_class_calculate_instance_members_count(*class, &members_length);
  //  Alloc
  object->members = sl_members_create(members_length);
  //  Assign options and member_ids
  _sl_class_init_instance_members(*class, object->members, 0);
  //  ref_count
  object->ref_count = 0;
  //  Iterating class tree
  return object;
}

sl_object_ref sl_object_retain(sl_object_ref object) {
  object->ref_count++;
  return object;
}

sl_object_ref sl_object_release(sl_object_ref object) {
  if (object->ref_count <= 1) {
    object->ref_count = 0;
    sl_object_destroy(object);
    return NULL;
  } else {
    object->ref_count--;
    return object;
  }
}

void sl_object_destroy(sl_object_ref object) {
  //  prevent loop
  if (object == NULL || object->deinitializing)
    return;
  object->deinitializing = sl_true;
  //  call custom deinitializer
  sl_call(sl_object_value(object), "deinit", 0);
  //  destroy members
  sl_members_destroy(object->members);
  //  free the object it self
  free(object);
}
