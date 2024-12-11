/*
    Copyright (C) 2018 Derrick Greenspan and the University of Central Florida

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#define SIZE_OF_MEM_WASTER 50

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// #include "library/llama.h"
#include <string.h>
#include <time.h>

void logo(void);
int main(void) {
  logo();
  int *var1 = malloc(sizeof(int) * SIZE_OF_MEM_WASTER);
  int i;

  /* Set each value of the array */
  for (i = 0; i < SIZE_OF_MEM_WASTER; i++) {
    var1[i] = i;
  }

  /*  Set one array equal to another */
  for (i = 0; i < SIZE_OF_MEM_WASTER; i++) {
    if (var1[i] < 10) {
      int *var2 = calloc(sizeof(int), SIZE_OF_MEM_WASTER);
      var2[i] = var1[i];
    }
  }

  /*  Set one array equal to another */
  int *var3 = calloc(sizeof(int), SIZE_OF_MEM_WASTER);
  for (i = 0; i < SIZE_OF_MEM_WASTER; i++) {
    var3[i] = var1[i];
  }

  /*  Perform interleaving accesses */
  int l = 0;
  for (i = 0; i < SIZE_OF_MEM_WASTER; i++) {
    var1[i] = l;
    /*  Perform a bunch of
     *  intervening operations */
    int j;
    for (j = 0; j < 1; j++) {
      int k;
      for (k = 0; k < 5; k++) {
        l = j * k;
      }
    }
  }
  printf("Done!\n");

  return 0;
}

void logo(void) {

  printf("=========================\n Branching Memory Allocator\n\tCALLOC "
         "EDITION\n=========================\n");
}
