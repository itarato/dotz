#include <cstdlib>
#include <ctime>

#include "app.h"

int main() {
  srand(time(nullptr));

  App().loop();
}
