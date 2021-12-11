#ifndef TRY_CATCH_H
#define TRY_CATCH_H
#include <setjmp.h>
#include <stdlib.h>

// used by macros for RAISE'd errors
extern jmp_buf * __TRY_CATCH_LIB__raise_env;
// used by macros for RETRY
extern jmp_buf * __TRY_CATCH_LIB__retry_env;

// list of predefined exception codes
enum {
  MALLOC_EXCEPTION = 0x80000000,// RAISE'd when malloc() == NULL in rmalloc()
  RETRY_EXCEPTION               // RAISE'd when RETRY not within a CATCH block
};

#define RAISE(exception) do {\
  if (__TRY_CATCH_LIB__raise_env)\
    longjmp(*__TRY_CATCH_LIB__raise_env,exception);\
  else\
    exit(exception);\
} while (0)

#define RETRY do {\
  if (__TRY_CATCH_LIB__retry_env)\
    longjmp(*__TRY_CATCH_LIB__retry_env,1);\
  else\
    RAISE(RETRY_EXCEPTION);\
} while(0)

#define TRY(block) do\
{\
  jmp_buf * const __TRY_CATCH_LIB__old_raise_env = __TRY_CATCH_LIB__raise_env;\
  jmp_buf * const __TRY_CATCH_LIB__old_retry_env = __TRY_CATCH_LIB__retry_env;\
  jmp_buf __TRY_CATCH_LIB__catch_raise_env;\
  int const __TRY_CATCH_LIB__reraise = setjmp(__TRY_CATCH_LIB__catch_raise_env);\
  \
  if (!__TRY_CATCH_LIB__reraise) {\
    jmp_buf __TRY_CATCH_LIB__try_raise_env;\
    int const __TRY_CATCH_LIB__exception = setjmp(__TRY_CATCH_LIB__try_raise_env);\
    jmp_buf __TRY_CATCH_LIB__try_retry_env;\
    int const __TRY_CATCH_LIB__retry  = setjmp(__TRY_CATCH_LIB__try_retry_env);\
    \
    __TRY_CATCH_LIB__raise_env = &__TRY_CATCH_LIB__try_raise_env; \
    __TRY_CATCH_LIB__retry_env = __TRY_CATCH_LIB__old_retry_env; \
    if (!__TRY_CATCH_LIB__exception || __TRY_CATCH_LIB__retry) {\
      block\
    }\
    else {\
      int const exception = __TRY_CATCH_LIB__exception;\
      \
      __TRY_CATCH_LIB__raise_env = &__TRY_CATCH_LIB__catch_raise_env;\
      __TRY_CATCH_LIB__retry_env = &__TRY_CATCH_LIB__try_retry_env;

#define CATCH(condition, block)\
      if (condition) {\
        block\
      }\
      else

#define EXCEPT(ex, block) CATCH(exception == ex, block)

#define FINALLY(block)\
      {\
        RAISE(exception);\
      }\
    }\
  }\
  __TRY_CATCH_LIB__retry_env = __TRY_CATCH_LIB__old_retry_env;\
  __TRY_CATCH_LIB__raise_env = __TRY_CATCH_LIB__old_raise_env;\
  block\
  if (__TRY_CATCH_LIB__reraise) {\
    RAISE(__TRY_CATCH_LIB__reraise);\
  }\
} while(0)

#endif
