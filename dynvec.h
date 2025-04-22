#ifndef DYNVEC_H
#define DYNVEC_H
#include <stddef.h>
#include <stdbool.h>
#include <time.h>

typedef struct Dynvec
{
    void *data;
    size_t size;
    size_t capacity;
    size_t length;
} dynvec;

typedef struct Limites{
    size_t left;
    size_t right;
} limites;
dynvec *dynvec_create(size_t size);
void resize(dynvec *v);
bool dynvec_contains(dynvec *v, void *elem, int (*cmp)(const void *, const void *));
bool dynvec_forall(dynvec *v, bool (*predicate)(void *));
void quicksort(dynvec *v, int (*cmp)(const void *, const void *));
void quicksort_dynvec(dynvec *v, int (*cmp)(const void *,const void *),size_t left, size_t right);
size_t partition_lomuto(dynvec *v, int (*cmp)(const void *, const void *), size_t left, size_t right);
void insertion_sort_dynvec(dynvec *v, int (*cmp)(const void *, const void *));
size_t partition(dynvec *v, int (*cmp)(const void *, const void *), size_t left, size_t right);
void swap(dynvec *v, size_t a, size_t b);
void heapsort_dynvec(dynvec *v, int (*cmp)(const void *,const void *));
void movedown(dynvec *v, size_t k, void *temp, size_t length, int (*cmp)(const void *, const void *));
void mergesort_dynvec(dynvec *v, int (*cmp)(const void *, const void *), size_t left, size_t right);
void quicksort_dynvec_three_way(dynvec *v, int (*cmp)(const void *, const void *), size_t left, size_t right);
limites partition_three_way(dynvec *v, int(*cmp)(const void*, const void *), size_t left, size_t right);
void mergesort(dynvec *v, int (*cmp)(const void *, const void *), size_t left, size_t right, size_t mid, void *temp);
size_t dynvec_exists_index(dynvec *v, bool (*predicate)(void *));
dynvec *dynvec_filter(dynvec *v, bool (*predicate)(void *));
bool dynvec_exists(dynvec *v, bool (*predicate)(void *));
size_t dynvec_index(dynvec *v, void *elem, int (*cmp)(const void *, const void *));
void dynvec_fold_left(dynvec *v, void *acc, void (*func)(void *acc, void *elem));
void dynvec_map(dynvec *v, void (*processo)(void *));
void dynvec_empty(dynvec *);
void *lsreach(dynvec *v, void *chave, int (*cmp)(const void *, const void *));
void *bsearch(dynvec *v, void *chave, int (*cmp)(const void *, const void *));
size_t dynvec_capacity(dynvec *);
size_t dynvec_length(dynvec *);
void dynvec_push(dynvec *, void *);
void *dynvec_pop(dynvec *);
int *dynvec_top(dynvec *);
void dynvec_set(dynvec *, size_t, void *);
void *dynvec_get(dynvec *, size_t);
void dynvec_insert(dynvec *, size_t, void *);
void dynvec_delete(dynvec *, size_t);
void dynvec_free(dynvec *);
#endif
