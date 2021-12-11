# Exception handling macros for C

This repository contains macros to enable OO-like exception handling in C.

It is shamelessly stolen from [rampion](https://gist.github.com/rampion/91594), with few changes -
just a Makefile for the example, and the documentation moved into this README.  Mostly because I
want this tiny lib in my back pocket.

## try-catch.h

### RAISE(exception)

When below a TRY block in the call stack, pass the given exception code to the paired CATCH blocks.

When no TRY block is above in the call stack, exits with the given error code.

NOTE: the given exception must be non-zero, otherwise, the behaviour is undefined

### RETRY

When below a CATCH block in the call stack, rerun the paired TRY block.

When no CATCH block is above in the call stack, raises a RETRY_EXCEPTION (0x80000001).

### TRY(block)

The TRY macro allows you to run the code passed in the given block, passing any RAISE'd exceptions
to paired CATCH blocks.

The TRY macro may be paired with zero or more CATCH blocks, but *must* be paired with a single
FINALLY block.

If an exception is RAISE'd, but not handled by any paired CATCH block, then it will be passed to
the next TRY/CATCH/FINALLY up the call stack after the paired FINALLY block is called.

```c
TRY(
  // some code
) FINALLY (
  // some code
);

TRY(
  // some code
) CATCH ( exception == 1,
  // what to do with a 1 exception
) CATCH ( exception == 2,
  // what to do with a 2 exception
  // ...
) FINALLY (
  // some code
);
```

See [try-catch-ex.c](./try-catch-ex.c) for a more comprehensive example.

### CATCH(condition, block)

The CATCH macro allows a block of code to be run conditionally on a matching exception being
RAISE'd within the paired TRY block.
 
The CATCH macro must be paired with a TRY block and a FINALLY block.

The condition and the block can access the `exception` variable to find the value of the most
recently RAISE'd exception.

The block will be run if:

 * no prior CATCH block's `condition` matches the exception
 * this `condition` evaluates to a non-zero value

If the CATCH block raises an exception, it will be passed to the next TRY/CATCH/FINALLY block up
the scope after the paired FINALLY block is called.

## rmalloc.c

### void* rmalloc(size_t const nbytes)

Convenience method to allocate nbytes.

returns a pointer to the allocated memory; RAISE's a MALLOC_EXCEPTION (0x80000000) if the memory
is not allocated, so it is not necessary to check the return value.

## Building the example

```bash
make
```