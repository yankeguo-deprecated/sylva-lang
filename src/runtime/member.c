//
// Created by Ryan on 2016/9/27.
//

#include "sylva/runtime.h"

#include <stdlib.h>
#include <string.h>
#include <sylva/runtime.h>

sylva_members_ref sylva_members_create(sylva_index length) {
  //  alloc
  sylva_members_ref members = malloc(sizeof(sylva_members));
  members->length = length;
  members->entries = malloc(sizeof(sylva_member) * length);
  //  clear
  for (sylva_index i = 0; i < length; i++) {
    members->entries[i].name = NULL;
    members->entries[i].option = sylva_member_normal;
    members->entries[i].value = sylva_nil_value;
  }
  return members;
}

sylva_index sylva_members_init(sylva_members_ref members, sylva_index start_idx, sylva_member_defs defs) {
  for (sylva_index i = 0; i < defs.length; i++) {
    members->entries[start_idx + i].name = defs.entries[i].name;
    members->entries[start_idx + i].option = defs.entries[i].option;
  }
  return defs.length;
}

sylva_value sylva_members_get(sylva_members members, sylva_symbol name) {
  for (sylva_index i = 0; i < members.length; i++) {
    if (sylva_symbol_equals(members.entries[i].name, name)) {
      return members.entries[i].value;
    }
  }
  return sylva_nil_value;
}

sylva_boolean sylva_members_set(sylva_members members, sylva_symbol name, sylva_value value) {
  for (sylva_index i = 0; i < members.length; i++) {
    if (sylva_symbol_equals(members.entries[i].name, name)) {
      //  if this is not a weak member, release the old value and retain the new value
      if (!(members.entries[i].option & sylva_member_weak)) {
        //  retain first then release, prevent accidentally release a reassigned object
        sylva_retain(&value);
        sylva_release(&members.entries[i].value);
      }
      members.entries[i].value = value;
      return sylva_true;
    }
  }
  return sylva_false;
}

void sylva_members_destroy(sylva_members_ref members) {
  for (sylva_index i = 0; i < members->length; i++) {
    if (!(members->entries[i].option & sylva_member_weak)) {
      sylva_release(&members->entries[i].value);
    }
  }
  free(members->entries);
  free(members);
}
