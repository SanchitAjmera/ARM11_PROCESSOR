#include "print_util.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {

  building_t *huxley = initialiseBuilding();
  char answer[100];
  printf("are you ready to battle KONSTANTINOS?\n");
  fflush(stdout);

  scanf("%s", answer);

  printKonstantinos();
  fflush(stdout);
  sleep(3);
  //  printBuildingDetails(huxley);
  for (int i = 0; i < 18; i++) {
    if (i % 6 == 0 || i % 6 == 1 || i % 6 == 2) {
      printKonstantinos();
      printRemaining();
      fflush(stdout);
      usleep(300000);
      printKonstantinosMouthOpen();
      printRemaining();
      fflush(stdout);
      usleep(300000);
    } else {
      printKonstantinosIndent();
      printRemaining();
      fflush(stdout);
      usleep(300000);
      printKonstantinosMouthOpenIndent();
      printRemaining();
      fflush(stdout);
      usleep(300000);
    }
  }
  freeBuilding(huxley);

  exit(EXIT_SUCCESS);
}
