#define _GNU_SOURCE
#include <fenv.h>

static void __attribute__ ((constructor))
set_fpu_exception_mask (void)
{
  feenableexcept (FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW);
}
