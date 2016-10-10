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

_SL_BEGIN_STD_C

sl_func_decl_class_static(Dog, main);
sl_func_decl_class_instance(Dog, name);
sl_func_decl_class_instance(Dog, init);
sl_func_decl_class_instance(Dog, bark);
sl_func_decl_module_instance(Printer, print);

sl_class_decl_start(Dog)
        sl_class_decl_super(Object)

        sl_class_decl_static_funcs_start(1)
                            sl_class_decl_static_class_func(Dog, main)
        sl_class_decl_static_funcs_end

        sl_class_decl_instance_funcs_start(4)
                            sl_class_decl_instance_class_func(Dog, name)
                            sl_class_decl_instance_class_func(Dog, bark)
                            sl_class_decl_instance_class_func(Dog, init)
                            sl_class_decl_instance_module_func(Printer, print)
        sl_class_decl_instance_funcs_end

        sl_class_decl_static_member_defs_start(1)
                            sl_class_decl_member_def(count, normal)
        sl_class_decl_static_member_defs_end

        sl_class_decl_instance_member_defs_start(1)
                            sl_class_decl_member_def(name, normal)
        sl_class_decl_instance_member_defs_end

        sl_class_decl_members_start(1)
                            sl_class_decl_member(count, normal)
        sl_class_decl_members_end
sl_class_decl_end

/////////////////////   Function Declarations   ///////////////////////

sl_func_decl_class_static(Dog, main) {
  sl_value dog = sl_create(&sl_class_name(Dog), "init", 1, sl_pointer_value("woff"));
  sl_retain(&dog);
  sl_call(dog, "bark", 0);
  sl_release(&dog);
  return sl_integer_value(0);
}

sl_func_decl_class_instance(Dog, name) {
  return sl_get(self, "name");
}

sl_func_decl_class_instance(Dog, init) {
  sl_value name = sl_args_get(arguments, 0);
  sl_set(self, "name", name);
  sl_value total_count = sl_static_get(self, "count");
  sl_value new_total_count = sl_call(total_count, "add", 1, sl_integer_value(1));
  sl_static_set(self, "count", new_total_count);
  return self;
}

sl_func_decl_class_instance(Dog, bark) {
  sl_value name = sl_call(self, "name", 0);
  sl_call(self, "print", 1, name);
  return sl_nil_value;
}

sl_func_decl_module_instance(Printer, print) {
  sl_value content = sl_args_get(arguments, 0);
  printf("%s:%s, I'm the number %ld dog\n",
         sl_get_class(self)->name,
         (char *) content.value.as_pointer,
         sl_static_get(self, "count").value.as_integer);
  return sl_nil_value;
}

////////////////////// Main Entry  //////////////////////////////

int main(_sl_unused int argc, _sl_unused char **argv) {
  return (int) sl_call(sl_class_rel(Dog), "main", 0).value.as_integer;
}

_SL_END_STD_C
