/**
  String HashMap Implementation
  Author: Naufal Fikri Setiawan (nsetiawan@student.unimelb.edu.au)
*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "hashmap.h"

#define KEY_SIZE 128


/**
  Creates a new hashmap.
  @param n_buckets the number of buckets in the hashtable.
  @param hash_function the function used to hash the key.
*/
HashMap *new_hashmap(int n_buckets, unsigned int (*hash_function)(char*)) {
  HashMap *h = malloc(sizeof(HashMap));
  assert(h);

  Bucket **b = malloc(n_buckets * sizeof(Bucket*));
  assert(b);

  h->buckets = b;
  h->hash = hash_function;
  h->size = n_buckets;

  return h;
}

/**
  Inserts a string item into the HashMap
  @param h the hashmap to insert the item to.
  @param item the string item to be inserted.
*/
void hashmap_insert(HashMap *h, char *key, int value) {
  unsigned int index = h->hash(key);
  index %= h->size;

  Bucket *current = h->buckets[index];
  Bucket *new_bucket = malloc(sizeof(Bucket));
  assert(new_bucket);

  new_bucket->key = malloc(KEY_SIZE * sizeof(char));
  strcpy(new_bucket->key, key);

  new_bucket->value = value;

  if (current != NULL) {
    new_bucket->next = current;
  }

  h->buckets[index] = new_bucket;
}

/**
  Looks up a string item inside the HashMap .
  @param h the hashmap to look for the item in.
  @param item the item to look for inside the hashmap.
*/
Bucket *hashmap_find(HashMap *h, char *key) {
  unsigned int index = h->hash(key);
  index %= h->size;

  Bucket *current = h->buckets[index];
  Bucket *previous = NULL;

  int found = 0;
  while (current != NULL) {
    if ((found = (strcmp(current->key, key) == 0))) {
      break;
    }

    previous = current;
    current = current->next;
  }

  if (found) {
    if (previous) {
      previous->next = current->next ? current->next : NULL;
      current->next = h->buckets[index];
      h->buckets[index] = current;
    } 
  }

  return found ? current : NULL;
}

/**
  Frees the HashMap from the memory.
  @param h the hashmap to free.
*/
void free_hashmap(HashMap *h) {
  for (int i = 0; i < h->size; ++i) {
    Bucket *current = h->buckets[i];
    while(current != NULL) {
      Bucket *next = current->next;
      free(current->key);
      free(current);
      current = next;
    }
  }

  free(h->buckets);
  free(h);
}