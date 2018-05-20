
#include "list.h"

unsigned int universal_hash(char *key);

HashMap *new_dictionary(int n_buckets);

HashMap *list_to_dictionary(List *list, int n_buckets);