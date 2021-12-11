#include <stdio.h>
#include "try-catch.h"
#include "rmalloc.h"

// Example of use for try-catch.h
int main(int argc, char *argv[])
{
  int i = 101;
  
  printf("before try block...\n");
  TRY ( 
      // a comment
      printf("before raising in try block (i = %d)...\n",i);
      RAISE(i-100);
      printf("after raising in try block...[ never run ]\n");
  ) CATCH ( exception < 5,
      printf("caught %d\n",exception); 
      // another comment
      i++;
      printf("retrying\n");
      RETRY;
  ) CATCH ( exception,
      printf("caught %d\n",exception); 
  ) FINALLY (
      printf("finally block\n");
  );
  printf("after try...\n");

  i = 105;
  TRY (
      printf("before retry outside of catch block (i = %d)...\n", i);
      RETRY; // illegal, will RAISE exception
      printf("after raising outside of catch block...[ never run ]\n");
  ) EXCEPT (RETRY_EXCEPTION,
      printf("caught RETRY_EXCEPTION\n");
      // vim :let c_no_curly_error=1 | e
      if (i < 106) {
        i = 106;
        RETRY;
      }
  ) FINALLY ();

  TRY (
      TRY (
          RETRY;
      ) EXCEPT (MALLOC_EXCEPTION,
        printf("will never be reached\n");
      ) FINALLY ();
  ) EXCEPT (RETRY_EXCEPTION,
    printf("Caught a nested exception\n");
  ) FINALLY ();

  printf("before raising outside of try block...\n");
  RAISE(1);
  printf("after raising outside of try block... should not see this\n");
}
