#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "search_functions.h"

/* 
 * Given a filename and a MAX_SIZExMAX_SIZE grid to fill, this function 
 * populates the grid and returns n, the actual grid dimension. 
 * If filename_to_read_from can't be opened, this function returns -1.
 * If the file contains an invalid grid, this function returns -2.
 */
int populate_grid(int *m, char grid[][MAX_SIZE], char filename_to_read_from[]){
  //initializing variables
  int row_len = 0;
  int num_rows = 0;
  char temp_row[MAX_SIZE + 1];
  int first_row = 0;

  //checks if file is able to be opened and returns error if not
  FILE* file_check = fopen(filename_to_read_from, "r");
  if (file_check == NULL) {
    printf("Grid file failed to open.\n");
    return -1;
  }

  //iterates through the rows getting the string at each until EOF encountered
  while (fgets(temp_row, sizeof(temp_row), file_check)) {

    //formats row and returns the length and length of the first row
    row_len = strlen(temp_row);
    if (temp_row[row_len - 1] == '\n') {
      temp_row[row_len - 1] = '\0';
      row_len--;
    }
    if(first_row == 0){
      first_row = row_len; 
    }
    //checks if there is a first row and returns error if not
    if(first_row == 0){
      printf("Invalid grid.\n");
      fclose(file_check);
      return -2;
    }

    //continues if an empty line encountered and then incriments rows
    if(temp_row[0] == '\0'){
      continue;
    }
    num_rows++;

    //checks that row length and num rows is correct size
    if(row_len > MAX_SIZE || row_len != first_row || num_rows > MAX_SIZE){
      printf("Invalid grid.\n");
      fclose(file_check);
      return -2;
    }
    //sets m value to row_len
    *m = row_len;

    //itereates through letters and checks if lowercase then puts them into the grid
    for(int col = 0; col < row_len; col++){
      if(!isalpha(temp_row[col]) || !islower(temp_row[col])) {
        printf("Invalid grid.\n");
        fclose(file_check);
        return -2;
      } else {
        grid[num_rows-1][col] = temp_row[col];
      }
    }
  }

  //prints out grid and information about the grid
  printf("num_rows = %d and num_cols = %d\n", num_rows, *m);
  print_grid(grid, num_rows, *m);

  //close file and return the number of rows
  fclose(file_check);
  return num_rows;
}


/* 
 * Given a number of rows, columns, a MAX_SIZExMAX_SIZE grid 
 * to fill, word to find and file to write to, this function 
 * finds the word in the grid in the diagonal down direction. 
 * Prints where the word was found and that it was found in DD direction.
 * Returns the number of times the word was found.
 */
int find_dd(char grid[][MAX_SIZE], int n, int m, char word[], FILE *write_to){
  //initialize variables
  int word_len = strlen(word);
  int word_found = 0;

  //iterates through rows only where word would still fit
  for(int row=0; row <= n-word_len; row++){
    //iterates through each letter in column only where word would still fit
    for(int col=0; col <= m-word_len; col++){
      //checks if current letter in grid matches first letter of word
      if(grid[row][col] == word[0]){
        //creates loop to continue checking if the letters match
        for(int i = 1; i <= word_len; i++){
          //checks if the whole word has been found and prints its location
          if(i == word_len){
            fprintf(write_to,"%s %d %d DD\n", word, row, col);
            word_found += 1;
            break;
          }
          //checks next letters
          if(grid[row+i][col+i] == word[i]){
            continue;
          } else {
            break;
          }
        }
      }
    }
  }
  //returns number of times word was found
  return word_found;
}


/* 
 * Given a number of rows, columns, a MAX_SIZExMAX_SIZE grid 
 * to fill, word to find and file to write to, this function 
 * finds the word in the grid in the diagonal up direction. 
 * Prints where the word was found and that it was found in DU direction.
 * Returns the number of times the word was found.
 */
int find_du (char grid[][MAX_SIZE], int n, int m, char word[], FILE *write_to){
  //initialize variables
  int word_len = strlen(word);
  int word_found = 0;
  char rev_word[word_len + 1];

  //reverse the word
  for(int i = 0; i < word_len; i++){
    rev_word[i] = word[word_len - 1 - i];
  }
  rev_word[word_len] = '\0';

  //iterates through rows only where word would still fit
  for(int row=0; row <= n-word_len; row++){
    //iterates through each letter in column only where word would still fit
    for(int col=0; col <= m-word_len; col++){
      //checks if current letter in grid matches first letter of word
      if(grid[row][col] == rev_word[0]){
        //creates loop to continue checking if the letters match
        for(int i = 1; i <= word_len; i++){
          //checks if the whole word has been found and prints its location
          if(i == word_len){
            fprintf(write_to,"%s %d %d DU\n", word, row+i-1, col+i-1);
            word_found += 1;
            break;
          }
          //checks next letters
          if(grid[row+i][col+i] == rev_word[i]){
            continue;
          } else {
            break;
          }
        }
      }
    }
  }
  //returns number of times word was found
  return word_found;
}


/* 
 * Given a number of rows, columns, a MAX_SIZExMAX_SIZE grid 
 * to fill, word to find and file to write to, this function 
 * finds the word in the grid in the anti-diagonal down direction. 
 * Prints where the word was found and that it was found in AD direction.
 * Returns the number of times the word was found.
 */
int find_ad (char grid[][MAX_SIZE], int n, int m, char word[], FILE *write_to){
  //initialize variables
  int word_len = strlen(word);
  int word_found = 0;

  //iterates through rows only where word would still fit
  for(int row=0; row <= n-word_len; row++){
    //iterates through each letter in column only where word would still fit
    for(int col=word_len-1; col <= m; col++){
      //checks if current letter in grid matches first letter of word
      if(grid[row][col] == word[0]){
        //creates loop to continue checking if the letters match
        for(int i = 1; i <= word_len; i++){
          //checks if the whole word has been found and prints its location
          if(i == word_len){
            fprintf(write_to,"%s %d %d AD\n", word, row, col);
            word_found += 1;
            break;
          }
          //checks next letters
          if(grid[row+i][col-i] == word[i]){
            continue;
          } else {
            break;
          }
        }
      }
    }
  }
  //returns number of times word was found
  return word_found;
}


/* 
 * Given a number of rows, columns, a MAX_SIZExMAX_SIZE grid 
 * to fill, word to find and file to write to, this function 
 * finds the word in the grid in the anti-diagonal up direction. 
 * Prints where the word was found and that it was found in AU direction.
 * Returns the number of times the word was found.
 */
int find_au (char grid[][MAX_SIZE], int n, int m, char word[], FILE *write_to){
  //initialize variables
  int word_len = strlen(word);
  int word_found = 0;
  char rev_word[word_len + 1];

  //reverse the word
  for(int i = 0; i < word_len; i++){
    rev_word[i] = word[word_len - 1 - i];
  }
  rev_word[word_len] = '\0';

  //iterates through rows only where word would still fit
  for(int row=0; row <= n-word_len; row++){
    //iterates through each letter in column only where word would still fit
    for(int col=word_len-1; col <= m; col++){
      //checks if current letter in grid matches first letter of word
      if(grid[row][col] == rev_word[0]){
        //creates loop to continue checking if the letters match
        for(int i = 1; i <= word_len; i++){
          //checks if the whole word has been found and prints its location
          if(i == word_len){
            fprintf(write_to,"%s %d %d AU\n", word, row+i-1, col-i+1);
            word_found += 1;
            break;
          }
          //checks next letters
          if(grid[row+i][col-i] == rev_word[i]){
            continue;
          } else {
            break;
          }
        }
      }
    }
  }
  //returns number of times word was found
  return word_found;
}


/* 
 * This function is similar to the other 4 find_ functions above, 
 * but reports ALL appearances of the given word, in the required 
 * DD, DU, AD, AU order.
 */
int find_all  (char grid[][MAX_SIZE], int n, int m, char word[], FILE *write_to) {

  //searches through the grid in all directions
  int dd_found = find_dd(grid, n, m, word, write_to);
  int du_found = find_du(grid, n, m, word, write_to);
  int ad_found = find_ad(grid, n, m, word, write_to);
  int au_found = find_au(grid, n, m, word, write_to);

  //totals the amount that the word was found
  int total_found = dd_found + du_found + ad_found + au_found;

  //prints out if word not found
  if(total_found == 0){
    fprintf(write_to,"%s - Not Found\n", word);
  }

  return -1; // replace this stub

} 


/*
 * Reads lhs and rhs character by character until either reaches eof.
 * Returns true if the files can be read and the two files match
 * character by character. Returns false if two files either can't be
 * opened or don't match. The definition of this function is provided 
 * for you.
 */
int file_eq(char lhs_name[], char rhs_name[]) {
  FILE* lhs = fopen(lhs_name, "r");
  FILE* rhs = fopen(rhs_name, "r");

  // don't compare if we can't open the files
  if (lhs == NULL || rhs == NULL) return 0;

  int match = 1;
  // read until both of the files are done or there is a mismatch
  while (!feof(lhs) || !feof(rhs)) {
	if (feof(lhs) ||                      // lhs done first
		feof(rhs) ||                  // rhs done first
		(fgetc(lhs) != fgetc(rhs))) { // chars don't match
	  match = 0;
	  break;
	}
  }
  fclose(lhs);
  fclose(rhs);
  return match;
}

/*
 * Helper function to print out the grid.
 */
void print_grid(char grid[][MAX_SIZE], int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (grid[i][j] != '\0')
	printf ("%c", grid[i][j]);
    }
    printf ("\n");
  }
}
