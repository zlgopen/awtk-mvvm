#include "mvvm/mvvm.h"

int main(void) {
  /* Link-time smoke only; runtime init needs a full AWTK application. */
  (void)mvvm_init;
  (void)mvvm_deinit;
  return 0;
}
