#pragma once

#define STRINGIFY(...) #__VA_ARGS__
#ifdef  __meta__
#define reflect __attribute__((annotate("__reflect__")))
#define full_reflect __attribute__((annotate("__full_reflect__")))
#define noreflect __attribute__((annotate("__noreflect__")))
#define attr(...) __attribute__((annotate(STRINGIFY(__VA_ARGS__))))
#define push_attr(...) __attribute__((annotate("__push__" STRINGIFY(__VA_ARGS__))))
#define pop_attr() __attribute__((annotate("__pop__")))
#else
#define reflect
#define full_reflect
#define noreflect
#define attr(...)
#define push_attr(...) 
#define pop_attr()
#endif