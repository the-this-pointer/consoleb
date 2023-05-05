#include <stdio.h>
#include "consoleb.h"
#include "consoleb_user.h"

int main() {
  consoleb_init();
  UserInterface_InitPages(&uih);

  consoleb_uninit();
  return 0;
}
