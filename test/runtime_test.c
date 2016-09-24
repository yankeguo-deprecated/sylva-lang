//
// runtime_test.c
//
// Created by Yanke Guo on 2016/9/24.
//

/* *
 *  dog.sylva
 *  ------------------------------------------
 *  class Dog
 *
 *    static func main
 *       self.new.bark
 *    end
 *
 *    func name
 *      return 1
 *    end
 *
 *    func bark
 *      var name = self.name
 *      // inline C printf self.name
 *    end
 *  end
 *
 *  dog.sylva - func_id table
 *  ------------------------------------------
 *  1 -> "name"
 *  2 -> "bark"
 *  3 -> "main"
 *
 * */

#include <sylva/runtime.h>

#include <stdio.h>

// Use macro to declare func names and ids for debug purpose
//
// func ids should be dynamically assigned by sylva compiler

#define FUNC_NAME_NAME "name"
#define FUNC_NAME_BARK "bark"
#define FUNC_NAME_MAIN "bark"

#define FUNC_ID_NAME 1
#define FUNC_ID_BARK 2
#define FUNC_ID_MAIN 3

/////////////////////   Function Declarations   ///////////////////////

sylva_value SYLV_Dog_S_main(sylva_value self, sylva_args args) {
  sylva_value dog = {
      .type = sylva_value_type_object,
      .object_value = &(sylva_object) {
          .class = self.class_value,
          .members = NULL,
      }
  };
  sylva_call(dog, FUNC_ID_BARK, 0);
  sylva_args_destroy(args);
  return sylva_value_integer(1);
}

sylva_value SYLV_Dog_I_name(sylva_value self, sylva_args args) {
  sylva_args_destroy(args);
  return sylva_value_integer(1);
}

sylva_value SYLV_Dog_I_bark(sylva_value self, sylva_args args) {
  sylva_value name = sylva_call(self, FUNC_ID_NAME, 0);
  printf("result: %ld", name.integer_value);
  sylva_args_destroy(args);
  return sylva_value_nil;
}

///////////////////////  Global Declarations ///////////////////////

sylva_func_id_registry sylva_runtime_func_id_registry = {
    .length = 2,
    .func_ids = (sylva_func_id[]) {FUNC_ID_NAME, FUNC_ID_BARK, FUNC_ID_MAIN},
    .func_names = (char *[]) {FUNC_NAME_NAME, FUNC_NAME_BARK, FUNC_NAME_MAIN},
};

sylva_member_id_registry sylva_runtime_member_id_registry = {
    .length = 0,
    .member_ids = NULL,
    .member_names = NULL,
};

sylva_class SYLV_Dog = (sylva_class) {
    .name = "Dog",
    .super = NULL,
    .modules = NULL,
    .modules_count = 0,
    .static_func_list = &(sylva_func_list) {
        .length = 1,
        .func_ids = (sylva_func_id[]) {FUNC_ID_MAIN},
        .funcs = (sylva_func[]) {&SYLV_Dog_S_main},
    },
    .instance_func_list = &(sylva_func_list) {
        .length = 2,
        .func_ids = (sylva_func_id[]) {FUNC_ID_NAME, FUNC_ID_BARK},
        .funcs = (sylva_func[]) {&SYLV_Dog_I_name, &SYLV_Dog_I_bark},
    },
    .static_member_list = NULL,
    .instance_member_list = NULL,
    .members = NULL,
};

////////////////////// Main Entry  //////////////////////////////

int main(int argc, char **argv) {
  sylva_call(sylva_value_class(&SYLV_Dog), 3, 0);
}
