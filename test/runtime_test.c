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
 *    static print
 *      print self->count
 *    end
 *  end
 *
 *  dog.sylva - func_id table
 *  ------------------------------------------
 *  1 -> "name"
 *  2 -> "bark"
 *  3 -> "main"
 *  4 -> "print"
 *
 * */

#include <sylva/runtime.h>
#include <sylva/foundation.h>

#include <stdio.h>

// Use macro to declare func names and ids for debug purpose
//
// func ids should be dynamically assigned by sylva compiler

#define FUNC_NAME_NAME "name"
#define FUNC_NAME_BARK "bark"
#define FUNC_NAME_MAIN "main"
#define FUNC_NAME_PRINT "print"
#define FUNC_NAME_INIT  "init"

#define MEMBER_NAME_COUNT "count"
#define MEMBER_NAME_NAME  "name"

#define FUNC_ID_NAME 1
#define FUNC_ID_BARK 2
#define FUNC_ID_MAIN 3
#define FUNC_ID_PRINT 4
#define FUNC_ID_INIT  5

#define MEMBER_ID_COUNT 1
#define MEMBER_ID_NAME  2

sylva_class SYLVA_C_Dog;

/////////////////////   Function Declarations   ///////////////////////

sylva_value SYLVA_C_Dog_S_main(sylva_value self, sylva_args args) {
  sylva_value dog = sylva_create(&SYLVA_C_Dog, FUNC_ID_INIT, 1, sylva_pointer_value("woff"));
  sylva_retain(&dog);
  sylva_call(dog, FUNC_ID_BARK, 0);
  sylva_release(&dog);
  return sylva_integer_value(0);
}

sylva_value SYLVA_C_Dog_I_name(sylva_value self, sylva_args args) {
  return sylva_members_get(*self.object_value->members, MEMBER_ID_NAME);
}

sylva_value SYLVA_C_Dog_I_init(sylva_value self, sylva_args args) {
  sylva_value name = sylva_args_get(args, 0);
  sylva_set(self, MEMBER_ID_NAME, name);
  sylva_value total_count = sylva_class_members_get(*self.object_value->class, MEMBER_ID_COUNT);
  sylva_value new_total_count = SYLVA_Number_I_add(total_count, sylva_args_make(1, sylva_integer_value(1)));
  sylva_set(sylva_class_value(sylva_get_class(self)), MEMBER_ID_COUNT, new_total_count);
  return self;
}

sylva_value SYLVA_C_Dog_I_bark(sylva_value self, sylva_args args) {
  sylva_value name = sylva_call(self, FUNC_ID_NAME, 0);
  sylva_call(self, FUNC_ID_PRINT, 1, name);
  return sylva_nil_value;
}

sylva_value SYLVA_M_Printer_I_print(sylva_value self, sylva_args args) {
  sylva_value content = sylva_args_get(args, 0);
  printf("%s:%s, I'm the number %ld dog\n",
         self.object_value->class->name,
         (char *) content.pointer_value,
         sylva_class_members_get(*self.object_value->class, MEMBER_ID_COUNT).integer_value);
  return sylva_nil_value;
}

///////////////////////  Global Declarations ///////////////////////

sylva_func_id_registry sylva_runtime_func_id_registry = {
    .length = 5,
    .func_ids = (sylva_func_id[]) {
        FUNC_ID_NAME,
        FUNC_ID_BARK,
        FUNC_ID_MAIN,
        FUNC_ID_PRINT,
        FUNC_ID_INIT
    },
    .func_names = (char *[]) {
        FUNC_NAME_NAME,
        FUNC_NAME_BARK,
        FUNC_NAME_MAIN,
        FUNC_NAME_PRINT,
        FUNC_NAME_INIT
    },
};

sylva_member_id_registry sylva_runtime_member_id_registry = {
    .length = 5,
    .member_ids = (sylva_member_id[]) {
        MEMBER_ID_COUNT,
        MEMBER_ID_NAME
    },
    .member_names = (char *[]) {
        MEMBER_NAME_COUNT,
        MEMBER_NAME_NAME
    },
};

sylva_class SYLVA_C_Dog = {
    .name = "Dog",
    .super = NULL,
    .static_funcs = &(sylva_funcs) {
        .length = 1,
        .func_ids = (sylva_func_id[]) {
            FUNC_ID_MAIN
        },
        .funcs = (sylva_func[]) {
            &SYLVA_C_Dog_S_main
        },
    },
    .instance_funcs = &(sylva_funcs) {
        .length = 4,
        .func_ids = (sylva_func_id[]) {
            FUNC_ID_NAME,
            FUNC_ID_BARK,
            FUNC_ID_PRINT,
            FUNC_ID_INIT
        },
        .funcs = (sylva_func[]) {
            &SYLVA_C_Dog_I_name,
            &SYLVA_C_Dog_I_bark,
            &SYLVA_M_Printer_I_print,
            &SYLVA_C_Dog_I_init
        },
    },
    .static_member_list = &(sylva_member_list) {
        .length = 1,
        .member_ids = (sylva_member_id[]) {
            MEMBER_ID_COUNT
        },
        .member_options = (sylva_member_option[]) {
            sylva_member_normal
        }
    },
    .instance_member_list = &(sylva_member_list) {
        .length = 1,
        .member_ids = (sylva_member_id[]) {
            MEMBER_ID_NAME
        },
        .member_options = (sylva_member_option[]) {
            sylva_member_normal
        }
    },
    .members = &(sylva_members) {
        .length = 1,
        .member_ids = (sylva_member_id[]) {
            MEMBER_ID_COUNT
        },
        .member_values = (sylva_value[]) {
            sylva_bare_integer_value(0)
        },
        .member_options = (sylva_member_option[]) {
            sylva_member_normal
        },
    },
};

////////////////////// Main Entry  //////////////////////////////

int main(int argc, char **argv) {
  char *name = sylva_runtime_func_id_to_name(FUNC_ID_PRINT);
  printf("Resolve FuncName: %s\n", name);
  return (int) sylva_call(sylva_class_value(&SYLVA_C_Dog), FUNC_ID_MAIN, 0).integer_value;
}
