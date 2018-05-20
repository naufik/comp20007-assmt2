/**
  String HashMap Implementation
  Author: Naufal Fikri Setiawan (nsetiawan@student.unimelb.edu.au)
*/

typedef struct bucket Bucket;

struct bucket {
  char *key;
  int value;
  Bucket *next;
};

typedef struct {
  unsigned int (*hash)(char*);
  int size;
  Bucket **buckets;
} HashMap;


HashMap *new_hashmap(int n_buckets, unsigned int (*hash_function)(char*));

void hashmap_insert(HashMap *h, char *key, int value);

Bucket *hashmap_find(HashMap *h, char *key);

void free_hashmap(HashMap *h);