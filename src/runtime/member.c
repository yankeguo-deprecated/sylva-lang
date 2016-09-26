//
// Created by Ryan on 2016/9/27.
//

#include "sylva/runtime.h"

#include <stdlib.h>
#include <string.h>

sylva_members_ref sylva_members_create(sylva_index length) {
  //  alloc
  sylva_members_ref members = malloc(sizeof(sylva_members));
  members->length = length;
  members->member_ids = malloc(sizeof(sylva_member_id) * length);
  members->member_options = malloc(sizeof(sylva_member_option) * length);
  members->member_values = malloc(sizeof(sylva_value) * length);
  //  clear
  for (sylva_index i = 0; i < length; i++) {
    members->member_ids[i] = 0;
    members->member_options[i] = sylva_member_normal;
    members->member_values[i] = sylva_value_nil;
  }
  return members;
}

sylva_index sylva_members_init(sylva_members_ref members,
                               sylva_index start_idx,
                               sylva_member_list member_list) {
  for (sylva_index i = 0; i < member_list.length; i++) {
    members->member_ids[start_idx + i] = member_list.member_ids[i];
    members->member_options[start_idx + 1] = member_list.member_options[i];
  }
  return member_list.length;
}

sylva_value sylva_members_get(sylva_members members, sylva_member_id member_id) {
  for (sylva_index i = 0; i < members.length; i++) {
    if (members.member_ids[i] == member_id) {
      return members.member_values[i];
    }
  }
  return sylva_value_nil;
}

sylva_boolean sylva_members_set(sylva_members members, sylva_member_id member_id, sylva_value value) {
  for (sylva_index i = 0; i < members.length; i++) {
    if (members.member_ids[i] == member_id) {
      //  if this is not a weak member, release the old value and retain the new value
      if (!(members.member_options[i] & sylva_member_weak)) {
        //  retain first then release, prevent accidentally release a reassigned object
        sylva_retain(&value);
        sylva_release(&members.member_values[i]);
      }
      members.member_values[i] = value;
      return sylva_true;
    }
  }
  return sylva_false;
}

void sylva_members_destroy(sylva_members_ref members) {
  for (sylva_index i = 0; i < members->length; i++) {
    if (!(members->member_options[i] & sylva_member_weak)) {
      sylva_release(&members->member_values[i]);
    }
  }
  free(members->member_ids);
  free(members->member_options);
  free(members->member_values);
  free(members);
}

char *sylva_member_id_registry_get_func_name(sylva_member_id_registry registry, sylva_member_id member_id) {
  for (sylva_index i = 0; i < registry.length; i++) {
    if (registry.member_ids[i] == member_id) {
      return registry.member_names[i];
    }
  }
  return NULL;
}

sylva_member_id sylva_member_id_registry_get_func_id(sylva_member_id_registry registry, char *member_name) {
  for (sylva_index i = 0; i < registry.length; i++) {
    if (strcmp(registry.member_names[i], member_name) == 0) {
      return registry.member_ids[i];
    }
  }
  return sylva_member_id_not_found;
}

char *sylva_runtime_member_id_to_name(sylva_member_id member_id) {
  return sylva_member_id_registry_get_func_name(sylva_runtime_member_id_registry, member_id);
}

sylva_member_id sylva_runtime_member_name_to_id(char *member_name) {
  return sylva_member_id_registry_get_func_id(sylva_runtime_member_id_registry, member_name);
}
