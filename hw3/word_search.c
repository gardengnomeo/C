#include <stdio.h>
#include "search_functions.h"

/*
 * This is the HW3 main function that performs a word search.
 */
int main(int argc, char* argv[]) {

  //checks if entered enough arguments
  if( argc < 2) {
    printf("Please enter a command line argument.\n");
    return 1;
  }    

  //intiitalizes variables and calls populate grid to get n
  int m;
  char grid[MAX_SIZE][MAX_SIZE];
  char word[MAX_SIZE];
  char n = populate_grid(&m, grid, argv[1]);

  //exits with return code if error raised in populate grid
  if(n < 0){
    return n;
  }

  //opens a file to write output to
  FILE *write_to = fopen("test.txt","w");
  
  //goes through words and finds all of them in the grid
  while(scanf(" %s", word) != EOF){
    find_all(grid, n, m, word, stdout);
  }
  
  fclose(write_to);
  return 0;

}
