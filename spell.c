/* 
  COMP20007 Design of Algorithms - Solution to Assignment 2
  Naufal F. Setiawan <nsetiawan@student.unimelb.edu.au>
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "list.h"
#include "spell.h"
#include "hashmap.h"
#include "dictionary.h"

#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define MAX_WORD_SIZE 128
#define INVALID -1

/** FUNCTION PROTOTYPES */
int edit_dist(char *word1, char *word2);
char **generate_edits(char *word);
unsigned int n_edits(char *word);
void free_string_array(char **strings, unsigned int n);
char *naive_dictionary_search(List *dictionary, char *string, 
  int min_dist, int max_dist);

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
  char **edits = generate_edits(word);
  int n = n_edits(word);

  for (unsigned int i = 0; i < n; ++i) {
    printf("%s\n", edits[i]);
  }

  free_string_array(edits, n);
}

// see Assignment Task 3: Spell checking
void print_checked(List *dictionary, List *document) {
  HashMap *d = list_to_dictionary(dictionary, 200000);

  Node *current = document->head;
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
// TODO: hashmap priorities
void print_corrected(List *dictionary, List *document) {
  HashMap *d = list_to_dictionary(dictionary, 200000);

  Node *current = document->head;
  while (current != NULL) {
    char *word = (char*)current->data;
      
      if (hashmap_find(d, word)) {
        printf("%s", word);
      } else {
        char **possible_edits = generate_edits(word);
        int n = n_edits(word);
        char *correction;

        int min_index = INVALID;
        for (int i = 0; i < n; ++i) {
          char *current_word = possible_edits[i];
          Bucket *entry = hashmap_find(d, current_word);

          if (entry) {
            if (min_index == INVALID || entry->value < min_index) {
              correction = entry->key;
              min_index = entry->value;
            }
            break;
          }
          if (min_index != INVALID) {
            printf("%s", correction);
          }
        }

        if (min_index == INVALID) {
          correction = naive_dictionary_search(dictionary, word,
            2, 3);
          if (correction != NULL) {
            printf("%s", correction);
          } else {
            printf("%s?", word);
          }
        }
        free_string_array(possible_edits, n_edits(word));
      }

    printf("\n");
    current = current->next;
  }

}


/** FUNCTIONS */

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

// generates all the possible edits.
char **generate_edits(char *word) {
  char **edits = malloc(n_edits(word) * sizeof(char*));
  unsigned int word_size = strlen(word);

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

  // removal edits.
  for (int i = 0; i < word_size; ++i) {
    memcpy(edits[current], word, i * sizeof(char));
    strcpy(&edits[current++][i], &word[i+1]);
  }

  // pre-insertion edits (can be optimized but i can't be fucked);
  for (int i = 0; ALPHABET[i] != '\0'; ++i) {
    for (int j = 0; j < word_size; ++j) {
      memcpy(edits[current], word, j * sizeof(char));
      edits[current][j] = ALPHABET[i];
      strcpy(&edits[current++][j+1], &word[j]);
    }
  }


  // post-insertion edits
  for (int i = 0; ALPHABET[i] != '\0'; ++i) {
    strcpy(edits[current], word);
    edits[current][word_size] = ALPHABET[i];
    edits[current++][word_size + 1] = '\0';
  }

  return edits;
}

// for freeing any string array with given size n. Used to free the 
// array of edits.
void free_string_array(char **strings, unsigned int n) {
  for (int i = 0; i < n; ++i) {
    free(strings[i]);
  }
  free(strings);
}

char *naive_dictionary_search(List *dictionary, char *string, 
  int min_dist, int max_dist) {

  int query_length = strlen(string);
  int n = dictionary->size;
  int best_dist = max_dist + 1;
  char *best_word = NULL;

  Node *dict_word = dictionary->head;
  for (int i = 0; i < n; ++i) {
    char *word = (char*)dict_word->data;

    // 
    if (abs(strlen(word) - query_length) > max_dist) {
      dict_word = dict_word->next;
      continue;
    }

    int dist = edit_dist(string, word);

    if (dist == min_dist) {
      return word;
    }

    if (dist <= max_dist && dist < best_dist) {
      best_dist = dist;
      best_word = word;
    }

    dict_word = dict_word->next;
  }

  return best_word;
}