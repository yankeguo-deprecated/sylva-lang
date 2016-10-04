//
// Created by Ryan on 2016/9/27.
//

#define __SYLVA_RUNTIME_SOURCE__
#include "sylva/runtime.h"

sl_members_ref sl_members_create(sl_index length) {
  //  alloc
  sl_members_ref members = malloc(sizeof(sl_members));
  members->length = length;
  members->entries = malloc(sizeof(sl_member) * length);
  //  clear
  for (sl_index i = 0; i < length; i++) {
    members->entries[i].name = NULL;
    members->entries[i].option = sl_member_normal;
    members->entries[i].value = sl_nil_value;
  }
  return members;
}

sl_index sl_members_init(sl_members_ref members, sl_index start_idx, sl_member_defs defs) {
  for (sl_index i = 0; i < defs.length; i++) {
    members->entries[start_idx + i].name = defs.entries[i].name;
    members->entries[start_idx + i].option = defs.entries[i].option;
  }
  return defs.length;
}

sl_value sl_members_get(sl_members members, sl_symbol name) {
  for (sl_index i = 0; i < members.length; i++) {
    if (sl_symbol_equals(members.entries[i].name, name)) {
      return members.entries[i].value;
    }
  }
  return sl_nil_value;
}

sl_boolean sl_members_set(sl_members members, sl_symbol name, sl_value value) {
  for (sl_index i = 0; i < members.length; i++) {
    if (sl_symbol_equals(members.entries[i].name, name)) {
      //  if this is not a weak member, release the old value and retain the new value
      if (!(members.entries[i].option & sl_member_weak)) {
        //  retain first then release, prevent accidentally release a reassigned object
        sl_retain(&value);
        sl_release(&members.entries[i].value);
      }
      members.entries[i].value = value;
      return sl_true;
    }
  }
  return sl_false;
}

void sl_members_destroy(sl_members_ref members) {
  for (sl_index i = 0; i < members->length; i++) {
    if (!(members->entries[i].option & sl_member_weak)) {
      sl_release(&members->entries[i].value);
    }
  }
  free(members->entries);
  free(members);
}
