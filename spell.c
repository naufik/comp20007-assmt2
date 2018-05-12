/* ... */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "spell.h"

int edit_dist(char* word1, char* word2);

int min(int a, int b) {
  return a < b ? a : b;
}

// see Assignment Task 1: Computing edit distance
void print_edit_distance(char *word1, char *word2) {
  printf("%d\n", edit_dist(word1, word2));
}

// see Assignment Task 2: Enumerating all possible edits
void print_all_edits(char *word) {
	printf("not yet implemented: put code for task 2 here\n");
}


// see Assignment Task 3: Spell checking
void print_checked(List *dictionary, List *document) {
	printf("not yet implemented: put code for task 3 here\n");
}

// see Assignment Task 4: Spelling correction
void print_corrected(List *dictionary, List *document) {
	printf("not yet implemented: put code for task 4 here\n");
}

// Calculates the edit distance between two words.
int edit_dist(char *word1, char *word2) {
  int table_width = strlen(word1) + 1;

  // We only need two active rows to do this.
  int *previous = malloc(sizeof(int) * table_width);
  int *current = malloc(sizeof(int) * table_width);

  // Populate the first row with the base case.
  for (int i = 0; i < table_width; ++i) {
    previous[i] = i;
  }

  for (int i = 0; word2[i] != '\0'; ++i) {
    // Base case for each word
    current[0] = 1 + i;

    for (int j = 0; word1[j] != '\0'; ++j) {

      // index that corresponds to current comparison (shifted by the empty)
      // string base case.
      int index = j + 1;

      // Calculate individual costs.
      int v_cost = previous[index] + 1;
      int h_cost = current[index - 1] + 1;
      int diag_cost = previous[index - 1] + (word2[i] != word1[j]);

      // Find the minimum one.
      current[index] = min(diag_cost, min(v_cost, h_cost));
    }

    if (word2[i + 1] != '\0') {
      int *temp = current;
      current = previous;
      previous = temp;
    }

    //shifts table
  }

  return current[table_width - 1];
}