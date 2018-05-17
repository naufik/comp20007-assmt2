/* 
  COMP20007 Design of Algorithms - Solution to Assignment 2
  Naufal F. Setiawan <nsetiawan@student.unimelb.edu.au>
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"
#include "spell.h"
#include "hashmap.h"
#include "dictionary.h"

#define ALPHABET "abcdefghijklmnopqrstuvwxyz"

/** FUNCTION PROTOTYPES */
int edit_dist(char* word1, char* word2);

/** FUNCTION DEFINITIONS */
int min(int a, int b) {
  return a < b ? a : b;
}

// see Assignment Task 1: Computing edit distance
void print_edit_distance(char *word1, char *word2) {
  printf("%d\n", edit_dist(word1, word2));
}

// see Assignment Task 2: Enumerating all possible edits
void print_all_edits(char *word) {
  for (int i = 0; word[i] != '\0'; ++i) {
    for (int j = 0; ALPHABET[j] != '\0'; ++j) {

      // printing all pre-insertion edits at i
      for (int k = 0; word[k] != '\0'; ++k) {
        if (k == i) {
          printf("%c", ALPHABET[j]);
        }
        printf("%c", word[k]);   
      }
      printf("\n");

      // printing substitution edits at i
      for (int k = 0; word[k] != '\0'; ++k) {
        printf("%c", k == i && word[k] != ALPHABET[j] ? ALPHABET[j] : word[k]);
      }
      printf("\n");

    }

    //printing deletion edits at i       
    for (int k = 0; word[k] != '\0'; ++k) {
      if (k != i) {
        printf("%c", word[k]);
      }
    }
    printf("\n");
  }

  // printing post-insertion edits
  for (int i = 0; ALPHABET[i] != '\0'; ++i) {
    printf("%s%c\n", word, ALPHABET[i]);
  }
}

// see Assignment Task 3: Spell checking
void print_checked(List *dictionary, List *document) {
  HashMap *d = new_dictionary(2000000);
  Node *current = dictionary->head;

  while (current != NULL) {
    hashmap_insert(d, (char*)current->data);
    current = current->next;
  }

  current = document->head;
  while (current != NULL) {
    char *word = (char*)current->data;
    printf("%s", word);
    if (!hashmap_find(d, word)) {
      printf("?");
    }
    printf("\n");
    current = current->next;
  }

  free_hashmap(d);
}

// see Assignment Task 4: Spelling correction
void print_corrected(List *dictionary, List *document) {
  //make dictionary hashmap
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

      // index that corresponds to current comparison (shifted by the empty
      // string base case).
      int index = j + 1;

      // Calculate individual costs for traversing horizontally or
      // vertically or diagonally.
      int v_cost = previous[index] + 1;
      int h_cost = current[index - 1] + 1;
      int diag_cost = previous[index - 1] + (word2[i] != word1[j]);

      // Find the minimum one.
      current[index] = min(diag_cost, min(v_cost, h_cost));
    }

    //shifts table
    if (word2[i + 1] != '\0') {
      int *temp = current;
      current = previous;
      previous = temp;
    }

  }

  int dist = current[table_width - 1];
  
  free(previous);
  free(current);
  return dist;
}

// given a word, calculates the number of edits possible when we
// have a set of alphabet letters.
unsigned int n_edits(char *word) {
  unsigned int str_size = strlen(word);
  unsigned int abc_size = strlen(ALPHABET);

  // pre-insertion + substitution + post-insertion + removal edits.
  return 2*(str_size*abc_size) + abc_size + str_size;
}

//
char **generate_edits(char *word) {
  char **edits = malloc(n_edits(word) * sizeof(char*));
  char word_size = strlen(word);

  // allocate memory for each string, this is gonna be a pain
  // in the neck to free()
  for (int i = 0; i < n_edits(word); ++i) {
    edits[i] = malloc(MAX_WORD_SIZE * sizeof(char));
  }

  int current = 0;

  // replacement edits
  for (int i = 0; ALPHABET[i] != '\0'; ++i) {
    for (int j = 0; j < word_size; ++j) {
      strcpy(edits[current], word);
      edits[current++][j] = ALPHABET[i];
    }
  }

  // post-insertion edits
  for (int i = 0; ALPHABET[i] != '\0'; ++i) {
    strcpy(edits[current], word);
    edits[current][word_size] = ALPHABET[i];
    edits[current++][word_size + 1] = '\0';
  }

  // removal edits.
  for (int i = 0; i < word_size; ++i) {
    memcpy(edits[current] * )
  }

  // pre-insertion edits (can be optimized but i can't be fucked);
  for (int i = 0; ALPHABET[i] != '\0'; ++i) {
    for (int j = 0; j < word_size; ++j) {

    }
  }
}