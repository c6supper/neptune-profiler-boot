#ifndef UNUSED_H_
#define UNUSED_H_

#define MON_Internal_UnusedStringify(macro_arg_string_literal) \
  #macro_arg_string_literal
#define MONUnusedParameter(macro_arg_parameter) \
  _Pragma(MON_Internal_UnusedStringify(unused(macro_arg_parameter)))

#endif  // UNUSED_H_
