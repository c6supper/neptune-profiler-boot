#ifndef TYPE_H_
#define TYPE_H_

#define INITIALIZER(f)                              \
  static void f(void) __attribute__((constructor)); \
  static void f(void)
#define DEINITIALIZER(f)                           \
  static void f(void) __attribute__((destructor)); \
  static void f(void)

#endif  // TYPE_H_
