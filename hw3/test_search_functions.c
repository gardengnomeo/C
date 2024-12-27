#include <stdio.h>
#include <assert.h>
#include "search_functions.h"


/* 
 * Declarations for tester functions whose definitions appear below.
 * (You will need to fill in the function definition details, at the
 * end of this file, and add comments to each one.) 
 * Additionally, for each helper function you elect to add to the 
 * provided search_functions.h, you will need to supply a corresponding
 * tester function in this file.  Add a declaration for it here, its
 * definition below, and a call to it where indicated in main.
 */
void test_file_eq();      // This one is already fully defined below.
void test_populate_grid();
void test_find_dd();
void test_find_du();
void test_find_ad();
void test_find_au();
void test_find_all();


/*
 * Main method which calls all test functions.
 */
int main() {
  printf("Testing file_eq...\n");
  test_file_eq();
  printf("Passed file_eq test.\n\n");

  printf("Running search_functions tests...\n");
  test_populate_grid();
  test_find_dd();
  test_find_du();
  test_find_ad();
  test_find_au();
  test_find_all();

  /* You may add calls to additional test functions here. */

  printf("Passed search_functions tests!!!\n");
}



/*
 * Test file_eq on same file, files with same contents, files with
 * different contents and a file that doesn't exist.
 * Relies on files test1.txt, test2.txt, test3.txt being present.
 */
void test_file_eq() {
  FILE* fptr = fopen("test1.txt", "w");
  fprintf(fptr, "this\nis\na test\n");
  fclose(fptr);

  fptr = fopen("test2.txt", "w");
  fprintf(fptr, "this\nis\na different test\n");
  fclose(fptr);

  fptr = fopen("test3.txt", "w");
  fprintf(fptr, "this\nis\na test\n");
  fclose(fptr);

  assert( file_eq("test1.txt", "test1.txt"));
  assert( file_eq("test2.txt", "test2.txt"));
  assert(!file_eq("test2.txt", "test1.txt"));
  assert(!file_eq("test1.txt", "test2.txt"));
  assert( file_eq("test3.txt", "test3.txt"));
  assert( file_eq("test1.txt", "test3.txt"));
  assert( file_eq("test3.txt", "test1.txt"));
  assert(!file_eq("test2.txt", "test3.txt"));
  assert(!file_eq("test3.txt", "test2.txt"));
  assert(!file_eq("", ""));  // can't open file
}


void test_populate_grid(){

  FILE* fptr = fopen("test4.txt", "w");
  fprintf(fptr, "this\ntest\nwill\npass\n");
  fclose(fptr);

  fptr = fopen("test5.txt", "w");
  fprintf(fptr, "this\ntest\nnot\npass\n");
  fclose(fptr);

  fptr = fopen("test6.txt", "w");
  fprintf(fptr, "thistestwontpas\nthistestwontpas\nthistestwontpas\n");
  fclose(fptr);

  fptr = fopen("test7.txt", "w");
  fprintf(fptr, "this\ntest\nwont\npass\nthis\ntest\nwont\npass\nthis\ntest\nwont\n");
  fclose(fptr);

  fptr = fopen("test8.txt", "w");
  fprintf(fptr, "thi5\nte5t\nwont\npass\n");
  fclose(fptr);

  fptr = fopen("test9.txt", "w");
  fprintf(fptr, "testpassed\ntestpassed\ntestpassed\ntestpassed\ntestpassed\ntestpassed\ntestpassed\ntestpassed\ntestpassed\ntestpassed\n");
  fclose(fptr);

  fptr = fopen("test10.txt", "w");
  fprintf(fptr, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  fclose(fptr);

  fptr = fopen("test11.txt", "w");
  fprintf(fptr, "test\nwill\npass\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  fclose(fptr);

  fptr = fopen("test12.txt", "w");
  fprintf(fptr, "test\nwill\npass");
  fclose(fptr);

  int m;
  char grid[MAX_SIZE][MAX_SIZE];
  
  assert( populate_grid(&m, grid, "test4.txt") == 4);
  assert( populate_grid(&m, grid, "test5.txt") == -2);
  assert( populate_grid(&m, grid, "test6.txt") == -2);
  assert( populate_grid(&m, grid, "test7.txt") == -2);
  assert( populate_grid(&m, grid, "test8.txt") == -2);
  assert( populate_grid(&m, grid, "test9.txt") == 10);
  assert( populate_grid(&m, grid, "test10.txt") == -2);
  assert( populate_grid(&m, grid, "test11.txt") == 3);
  assert( populate_grid(&m, grid, "test12.txt") == 3);
  assert( populate_grid(&m, grid, "test13.txt") == -1);

}


void test_find_dd(){

  char grid[MAX_SIZE][MAX_SIZE] = {
    {'s', 'x', 'a', 'o', 'o'},
    {'l', 'y', 'x', 'f', 'x'},
    {'w', 'k', 'r', 'y', 'c'},
    {'j', 'u', 'v', 'a', 'd'}
  };
  int n = 4;
  int m = 5;
  char test1[] = "syra";
  char test2[] = "syrap";
  char test3[] = "o";

  FILE* fptr = fopen("test14.txt", "w");
  fprintf(fptr, "syra 0 0 DD\n");
  fclose(fptr);

  fptr = fopen("test15.txt", "w");
  fclose(fptr);

  fptr = fopen("test16.txt", "w");
  fprintf(fptr, "o 0 3 DD\no 0 4 DD\n");
  fclose(fptr);

  FILE* output1 = fopen("output1.txt", "w");
  FILE* output2 = fopen("output2.txt", "w");
  FILE* output3 = fopen("output3.txt", "w");

  find_dd(grid, n, m, test1, output1);
  fclose(output1);
  find_dd(grid, n, m, test2, output2);
  fclose(output2);
  find_dd(grid, n, m, test3, output3);
  fclose(output3);

  assert( file_eq("test14.txt", "output1.txt"));
  assert( file_eq("test15.txt", "output2.txt"));
  assert( file_eq("test16.txt", "output3.txt"));

}


void test_find_du(){

  char grid[MAX_SIZE][MAX_SIZE] = {
    {'s', 'x', 'a', 'o', 'o'},
    {'l', 'y', 'x', 'f', 'x'},
    {'w', 'k', 'r', 'y', 'c'},
    {'j', 'u', 'v', 'a', 'd'}
  };
  int n = 4;
  int m = 5;
  char test4[] = "yxx";
  char test5[] = "sway";
  char test6[] = "o";

  FILE* fptr = fopen("test17.txt", "w");
  fprintf(fptr, "yxx 2 3 DU\n");
  fclose(fptr);

  fptr = fopen("test18.txt", "w");
  fclose(fptr);

  fptr = fopen("test19.txt", "w");
  fprintf(fptr, "o 0 3 DU\no 0 4 DU\n");
  fclose(fptr);

  FILE* output4 = fopen("output4.txt", "w");
  FILE* output5 = fopen("output5.txt", "w");
  FILE* output6 = fopen("output6.txt", "w");

  find_du(grid, n, m, test4, output4);
  fclose(output4);
  find_du(grid, n, m, test5, output5);
  fclose(output5);
  find_du(grid, n, m, test6, output6);
  fclose(output6);

  assert( file_eq("test17.txt", "output4.txt"));
  assert( file_eq("test18.txt", "output5.txt"));
  assert( file_eq("test19.txt", "output6.txt"));

}


void test_find_ad(){

  char grid[MAX_SIZE][MAX_SIZE] = {
    {'s', 'x', 'a', 'o', 'o'},
    {'l', 'y', 'x', 'f', 'x'},
    {'w', 'k', 'r', 'y', 'c'},
    {'j', 'u', 'v', 'a', 'd'}
  };
  int n = 4;
  int m = 5;
  char test7[] = "ayw";
  char test8[] = "sway";
  char test9[] = "o";

  FILE* fptr = fopen("test20.txt", "w");
  fprintf(fptr, "ayw 0 2 AD\n");
  fclose(fptr);

  fptr = fopen("test21.txt", "w");
  fclose(fptr);

  fptr = fopen("test22.txt", "w");
  fprintf(fptr, "o 0 3 AD\no 0 4 AD\n");
  fclose(fptr);

  FILE* output7 = fopen("output7.txt", "w");
  FILE* output8 = fopen("output8.txt", "w");
  FILE* output9 = fopen("output9.txt", "w");

  find_ad(grid, n, m, test7, output7);
  fclose(output7);
  find_ad(grid, n, m, test8, output8);
  fclose(output8);
  find_ad(grid, n, m, test9, output9);
  fclose(output9);

  assert( file_eq("test20.txt", "output7.txt"));
  assert( file_eq("test21.txt", "output8.txt"));
  assert( file_eq("test22.txt", "output9.txt"));

}


void test_find_au(){

  char grid[MAX_SIZE][MAX_SIZE] = {
    {'s', 'x', 'a', 'o', 'o'},
    {'l', 'y', 'x', 'f', 'x'},
    {'w', 'k', 'r', 'y', 'c'},
    {'j', 'u', 'v', 'a', 'd'}
  };
  int n = 4;
  int m = 5;
  char test7[] = "vyx";
  char test8[] = "sway";
  char test9[] = "o";

  FILE* fptr = fopen("test23.txt", "w");
  fprintf(fptr, "vyx 3 2 AU\n");
  fclose(fptr);

  fptr = fopen("test24.txt", "w");
  fclose(fptr);

  fptr = fopen("test25.txt", "w");
  fprintf(fptr, "o 0 3 AU\no 0 4 AU\n");
  fclose(fptr);

  FILE* output7 = fopen("output7.txt", "w");
  FILE* output8 = fopen("output8.txt", "w");
  FILE* output9 = fopen("output9.txt", "w");

  find_au(grid, n, m, test7, output7);
  fclose(output7);
  find_au(grid, n, m, test8, output8);
  fclose(output8);
  find_au(grid, n, m, test9, output9);
  fclose(output9);

  assert( file_eq("test23.txt", "output7.txt"));
  assert( file_eq("test24.txt", "output8.txt"));
  assert( file_eq("test25.txt", "output9.txt"));

}


void test_find_all(){

  char grid[MAX_SIZE][MAX_SIZE] = {
    {'s', 'x', 'a', 'o', 'o'},
    {'l', 'y', 'x', 'f', 'x'},
    {'w', 'k', 'r', 'y', 'c'},
    {'j', 'u', 'v', 'a', 'd'}
  };
  int n = 4;
  int m = 5;
  char test7[] = "o";
  char test8[] = "sway";
  char test9[] = "xx";

  FILE* fptr = fopen("test26.txt", "w");
  fprintf(fptr, "o 0 3 DD\no 0 4 DD\no 0 3 DU\no 0 4 DU\no 0 3 AD\no 0 4 AD\no 0 3 AU\no 0 4 AU\n");
  fclose(fptr);

  fptr = fopen("test27.txt", "w");
  fprintf(fptr, "sway - Not Found\n");
  fclose(fptr);

  fptr = fopen("test28.txt", "w");
  fprintf(fptr, "xx 0 1 DD\nxx 1 2 DU\n");
  fclose(fptr);

  FILE* output7 = fopen("output7.txt", "w");
  FILE* output8 = fopen("output8.txt", "w");
  FILE* output9 = fopen("output9.txt", "w");

  find_all(grid, n, m, test7, output7);
  fclose(output7);
  find_all(grid, n, m, test8, output8);
  fclose(output8);
  find_all(grid, n, m, test9, output9);
  fclose(output9);

  assert( file_eq("test26.txt", "output7.txt"));
  assert( file_eq("test27.txt", "output8.txt"));
  assert( file_eq("test28.txt", "output9.txt"));

}
