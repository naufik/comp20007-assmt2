/**
  String HashMap Implementation
  Author: Naufal Fikri Setiawan (nsetiawan@student.unimelb.edu.au)
*/

typedef struct {
  int (*hash)(char*);
  int size;
  Bucket **buckets;
} HashMap;

typedef struct bucket Bucket;

struct bucket {
  char *content;
  Bucket *next;
};

HashMap *new_hashmap(int n_buckets, int (*hash_function)(char*)) {
  HashMap *h = malloc(sizeof(HashMap));
  assert(h);

  Bucket *b = malloc(n_buckets * sizeof(Bucket*));
  assert(b);

  h->buckets = b;
  h->hash = hash_function;
  h->size = n_buckets;

  return h;
}

void hashmap_insert(HashMap *h, char *item) {
  int index = h->hash(item);
  index %= n_buckets;

  Bucket *current = h->buckets[index];
  Bucket *new_bucket = malloc(sizeof(Bucket));
  assert(new_bucket);

  strcpy(new_bucket->content, item);

  if (current != NULL) {
    new_bucket->next = current;
  }

  h->buckets[index] = new_bucket;
}

Bucket *hashmap_find(HashMap *h, char* item) {
  int index = h->hash(item);
  index %= n_buckets;

  Bucket *current = h->buckets[index];
  int found = 0;

  while (current == NULL || !(found = strcmp(current->content, item))) {
    current = current->next;
  }

  return found ? current : NULL;
}