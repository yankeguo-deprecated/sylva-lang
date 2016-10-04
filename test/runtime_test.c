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

sl_class SYLVA_C_Dog;

/////////////////////   Function Declarations   ///////////////////////

sl_value SYLVA_C_Dog_S_main(sl_value self, sl_args args) {
  sl_value dog = sl_create(&SYLVA_C_Dog, "init", 1, sl_pointer_value("woff"));
  sl_retain(&dog);
  sl_call(dog, "bark", 0);
  sl_release(&dog);
  return sl_integer_value(0);
}

sl_value SYLVA_C_Dog_I_name(sl_value self, sl_args args) {
  return sl_get(self, "name");
}

sl_value SYLVA_C_Dog_I_init(sl_value self, sl_args args) {
  sl_value name = sl_args_get(args, 0);
  sl_set(self, "name", name);
  sl_value total_count = sl_static_get(self, "count");
  sl_value new_total_count = sl_call(total_count, "add", 1, sl_integer_value(1));
  sl_static_set(self, "count", new_total_count);
  return self;
}

sl_value SYLVA_C_Dog_I_bark(sl_value self, sl_args args) {
  sl_value name = sl_call(self, "name", 0);
  sl_call(self, "print", 1, name);
  return sl_nil_value;
}

sl_value SYLVA_M_Printer_I_print(sl_value self, sl_args args) {
  sl_value content = sl_args_get(args, 0);
  printf("%s:%s, I'm the number %ld dog\n",
         sl_get_class(self)->name,
         (char *) content.pointer_value,
         sl_static_get(self, "count").integer_value);
  return sl_nil_value;
}

///////////////////////  Global Declarations ///////////////////////

sl_class SYLVA_C_Dog = {
    .name = "Dog",
    .super = &SYLVA_Object,
    .static_funcs =
    &sl_funcs_make(1,
                   sl_func_item("main", &SYLVA_C_Dog_S_main)
    ),
    .instance_funcs =
    &sl_funcs_make(4,
                   sl_func_item("name", &SYLVA_C_Dog_I_name),
                   sl_func_item("bark", &SYLVA_C_Dog_I_bark),
                   sl_func_item("print", &SYLVA_M_Printer_I_print),
                   sl_func_item("init", &SYLVA_C_Dog_I_init),
    ),
    .static_member_defs =
    &sl_member_defs_make(1,
                         sl_member_def_item("count", sl_member_normal)
    ),
    .instance_member_defs =
    &sl_member_defs_make(1,
                         sl_member_def_item("name", sl_member_normal)
    ),
    .members =
    &sl_members_make(1,
                     sl_member_item("count", sl_member_normal)
    )
};

////////////////////// Main Entry  //////////////////////////////

int main(int argc, char **argv) {
  return (int) sl_call(sl_class_value(&SYLVA_C_Dog), "main", 0).integer_value;
}
