//
// runtime_test.c
//
// Created by Yanke Guo on 2016/9/24.
//

/* *
 *  dog.sylva
 *  ------------------------------------------
 *  module Printer
 *    def print(x)
 *      // inline C code printf
 *    end
 *  end
 *
 *  class Dog
 *    include Printer
 *    static var count
 *    var name
 *
 *    static func main
 *       self.new("wof").bark
 *    end
 *
 *    func init(name)
 *      self->name = name
 *    end
 *
 *    func name
 *      return self->name
 *    end
 *
 *    func bark
 *      print self.name
 *    end
 *
 *  end
 *
 * */

#include <sylva/runtime.h>
#include <sylva/foundation.h>

#include <stdio.h>

sylva_class SYLVA_C_Dog;

/////////////////////   Function Declarations   ///////////////////////

sylva_value SYLVA_C_Dog_S_main(sylva_value self, sylva_args args) {
  sylva_value dog = sylva_create(&SYLVA_C_Dog, "init", 1, sylva_pointer_value("woff"));
  sylva_retain(&dog);
  sylva_call(dog, "bark", 0);
  sylva_release(&dog);
  return sylva_integer_value(0);
}

sylva_value SYLVA_C_Dog_I_name(sylva_value self, sylva_args args) {
  return sylva_get(self, "name");
}

sylva_value SYLVA_C_Dog_I_init(sylva_value self, sylva_args args) {
  sylva_value name = sylva_args_get(args, 0);
  sylva_set(self, "name", name);
  sylva_value total_count = sylva_static_get(self, "count");
  sylva_value new_total_count = SYLVA_Number_I_add(total_count, sylva_args_make(1, sylva_integer_value(1)));
  sylva_static_set(self, "count", new_total_count);
  return self;
}

sylva_value SYLVA_C_Dog_I_bark(sylva_value self, sylva_args args) {
  sylva_value name = sylva_call(self, "name", 0);
  sylva_call(self, "print", 1, name);
  return sylva_nil_value;
}

sylva_value SYLVA_M_Printer_I_print(sylva_value self, sylva_args args) {
  sylva_value content = sylva_args_get(args, 0);
  printf("%s:%s, I'm the number %ld dog\n",
         sylva_get_class(self)->name,
         (char *) content.pointer_value,
         sylva_static_get(self, "count").integer_value);
  return sylva_nil_value;
}

///////////////////////  Global Declarations ///////////////////////

sylva_class SYLVA_C_Dog = {
    .name = "Dog",
    .super = &SYLVA_Object,
    .static_funcs =
    &sylva_funcs_make(1,
                      sylva_func_item("main", &SYLVA_C_Dog_S_main)
    ),
    .instance_funcs =
    &sylva_funcs_make(4,
                      sylva_func_item("name", &SYLVA_C_Dog_I_name),
                      sylva_func_item("bark", &SYLVA_C_Dog_I_bark),
                      sylva_func_item("print", &SYLVA_M_Printer_I_print),
                      sylva_func_item("init", &SYLVA_C_Dog_I_init),
    ),
    .static_member_defs =
    &sylva_member_defs_make(1,
                            sylva_member_def_item("count", sylva_member_normal)
    ),
    .instance_member_defs =
    &sylva_member_defs_make(1,
                            sylva_member_def_item("name", sylva_member_normal)
    ),
    .members =
    &sylva_members_make(1,
                        sylva_member_item("count", sylva_member_normal)
    )
};

////////////////////// Main Entry  //////////////////////////////

int main(int argc, char **argv) {
  return (int) sylva_call(sylva_class_value(&SYLVA_C_Dog), "main", 0).integer_value;
}
