#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "dynvec.h"
#include <stdbool.h>
#include <time.h>
#define DYNVEC_INIT_CAPACITY 4

dynvec *dynvec_create(size_t size)
{
    dynvec *v = (dynvec *)malloc(sizeof(dynvec));
    if (!v)
    {
        errno = ENOMEM;
#ifdef DEBUG_ON
        perror("Create. Erro ao alocar memória");
#endif
        return NULL;
    }

    v->capacity = DYNVEC_INIT_CAPACITY;

    v->length = 0;
    v->size = size;
    v->data = malloc(v->capacity * v->size);
    if (!(v->data))
    {
        errno = ENOMEM;
#ifdef DEBUG_ON
        perror("Create. Erro no array, ao alocar memória");
#endif
        free(v);
        return NULL;
    }
    return v;
}
void dynvec_fold_left(dynvec *v, void *acc, void (*func)(void *acc, void *elem))
{
    if (!v || !func)
    {
        return;
    }
    for (size_t i = 0; i < v->length; i++)
    {
        func(acc, dynvec_get(v, i));
    }
}
size_t dynvec_index(dynvec *v, void *elem, int (*cmp)(const void *, const void *))
{
    if (!v || !cmp)
    {
        return -1;
    }
    for (size_t i = 0; i < v->length; i++)
    {
        if (cmp((char *)v->data + (i * v->size), elem) == 0)
        {
            return i;
        }
    }
    return -1;
}
void mergesort_dynvec(dynvec *v, int (*cmp)(const void *, const void *), size_t left, size_t right)
{

    if (!v || !cmp || !v->data || v->size == 0)
    {
        return;
    }
    if (right - left <= 1)
    {
        return;
    }

    size_t mid = left + (right - left) / 2;
    void *temp = malloc((right - left) * v->size);
    if (!temp)
    {
        return;
    }

    mergesort_dynvec(v, cmp, left, mid);
    mergesort_dynvec(v, cmp, mid, right);
    memcpy(temp, (char *)v->data + left * v->size, (right - left) * v->size);
    mergesort(v, cmp, left, right, mid, temp);
    free(temp);
}
void mergesort(dynvec *v, int (*cmp)(const void *, const void *), size_t left, size_t right, size_t mid, void *temp)

{
    if (!v || !cmp || !v->data || v->size == 0)
    {
        return;
    }
    int i = left, j = mid;
    for (size_t k = left; k < right; k++)
    {
        if (i < mid && (j == right || (cmp((char *)temp + ((i - left) * v->size), (char *)temp + ((j - left) * v->size)) <= 0)))
        {
            memmove((char *)v->data + k * v->size, (char *)temp + ((i - left) * v->size), v->size);
            i++;
        }
        else
        {
            memmove((char *)v->data + k * v->size, (char *)temp + ((j - left) * v->size), v->size);
            j++;
        }
    }
}
limites partition_three_way(dynvec *v, int(*cmp)(const void*, const void *), size_t left, size_t right)
{
    limites limite={0,0};
    if (!v || !cmp || !v->data || v->size == 0)
    {
        return;
    }
    size_t pivot = left + rand() % (right - left + 1);
    swap(v, pivot, left);
    size_t i=left+1, r=right,l=left+1;
    while (i<=r)
    {
        if (cmp((char *)v->data + i * v->size, (char *)v->data + left * v->size) < 0)
        {
            swap(v, i, l);
            l++;
            i++;
        }
        else if (cmp((char *)v->data + i * v->size, (char *)v->data + left * v->size) > 0)
        {
            swap(v, i, r);
            r--;
        }
        else
        {
         i++;
        }
    }
    swap(v,l-1,left);
    limite.left=l-1;
    limite.right=r+1;
    return limite;

}
void quicksort_dynvec_three_way(dynvec *v, int (*cmp)(const void *, const void *), size_t left, size_t right)
{
    if (!v || !cmp || !v->data || v->size == 0)
    {
        return;
    }
    if (left < right)
    {
        limites p = partition_three_way(v, cmp, left, right);

        if (p.left > left)
        {
            quicksort_dynvec_three_way(v, cmp, left, p.left - 1);
        }
        if (p.right< right)
        {
            quicksort_dynvec_three_way(v, cmp, p.right, right);
        }
    }
}
size_t partition_media_of_three(dynvec *v, int (*cmp)(const void *, const void *), size_t left, size_t right){
    if (!v || !cmp || !v->data || v->size == 0)
    {
        return 0;
    }
    size_t m=left;
    size_t pivo_index;
    void *left1=(char *)v->data + left * v->size,*mid=(char *)v->data + (dynvec_length(v)/2) * v->size,*right1=(char *)v->data + right * v->size;
    void *pivo;
    if ((cmp(left1,right1)>=0 && cmp(left1,mid)<=0) || cmp(left1,right1)<=0 &&cmp(left1,mid)>=0)
    {
        pivo=left1;
        pivo_index=left;
    }
    else if (cmp(mid,right1)>=0 && cmp(mid,left1)<=0 || cmp(mid,right1)<=0 && cmp(mid,left1)>=0)
    {
        pivo=mid;
        pivo_index=(dynvec_length(v)/2);
    }
    else
    {
        pivo=right1;
        pivo_index=right;
    }
    for (size_t i = left; i < right; i++)
    {
        if (cmp((char *)v->data + i * v->size,pivo) < 0)
        {
            swap(v, i, m);
            m++;  //m guarda o sitio onde o proximo elemento mais pequeno vai ser colocado
        }
    }
    swap(v, m, pivo_index);
    return m;
    
}
size_t partition_lomuto(dynvec *v, int (*cmp)(const void *, const void *), size_t left, size_t right)
{
    if (!v || !cmp || !v->data || v->size == 0)
    {
        return 0;
    }
    size_t m=left; 
    void *pivo=(char *)v->data + right * v->size;
    for (size_t i = left; i < right; i++)
    {
        if (cmp((char *)v->data + i * v->size,pivo) < 0)
        {
            swap(v, i, m);
            m++;  //m guarda o sitio onde o proximo elemento mais pequeno vai ser colocado
        }  
    }
    swap(v, m, right); 
    return m;
}
void quicksort(dynvec *v, int (*cmp)(const void *, const void *)){
    if (!v || !v->data || v->size == 0 || v->length == 0)
    {
        return;
    }
    quicksort_dynvec(v, cmp, 0, v->length - 1);
}
void quicksort_dynvec(dynvec *v, int (*cmp)(const void *, const void *), size_t left, size_t right)
{
    if (!v || !cmp || !v->data || v->size == 0)
    {
        return;
    }
    if (left < right)
    {
        size_t m = partition(v, cmp, left, right);

        if (m > left)
        {
            quicksort_dynvec(v, cmp, left, m - 1);
        }
        if (m < right)
        {
            quicksort_dynvec(v, cmp, m + 1, right);
        }
    }
}
size_t partition(dynvec *v, int (*cmp)(const void *, const void *), size_t left, size_t right)
{
    if (!v || !cmp || !v->data || v->size == 0)
    {
        return 0;
    }
    size_t pivot = left + rand() % (right - left + 1);
    swap(v, pivot, left); // pivo passa a estar no inicio do vetor
    size_t m = left;
    for (size_t i = left + 1; i <= right; i++)
    {
        if (cmp((char *)v->data + i * v->size, (char *)v->data + left * v->size) < 0)
        {
            m++;
            swap(v, i, m);
        }
    }
    swap(v, left, m);
    return m;
}
void swap(dynvec *v, size_t a, size_t b)
{
    if (!v || !v->data || v->size == 0)
    {
        return;
    }
    void *temp = malloc(v->size);
    if (!temp)
    {
        return;
    }
    memcpy(temp, (char *)v->data + a * v->size, v->size);
    memcpy((char *)v->data + a * v->size, (char *)v->data + b * v->size, v->size);
    memcpy((char *)v->data + b * v->size, temp, v->size);
    free(temp);
}

void insertion_sort_dynvec(dynvec *v, int (*cmp)(const void *, const void *))
{
    if (!v || !cmp || !v->data || v->size == 0)
    {
        return;
    }

    void *temp = malloc(v->size);
    if (!temp)
    {
        return;
    }

    for (size_t i = 1; i < v->length; i++)
    {
        size_t j = i;

        memcpy(temp, (char *)v->data + (i * v->size), v->size);
        for (; j > 0 && cmp((char *)v->data + ((j - 1) * v->size), temp) > 0; j--)
        {
            memmove((char *)v->data + (j * v->size), (char *)v->data + ((j - 1) * v->size), v->size);
        }
        memcpy((char *)v->data + (j * v->size), temp, v->size);
    }
    free(temp);
}
void movedown(dynvec *v, size_t k, void *temp, size_t length, int (*cmp)(const void *, const void *))
{
    while (true)
    {
        int j = 2 * k + 1;
        if (j >= length)
        {
            break;
        }
        if (j + 1 < length && cmp((char *)v->data + (j * v->size), (char *)v->data + ((j + 1) * v->size)) < 0)
        {
            j++;
        }
        if (cmp(temp, (char *)v->data + (j * v->size)) >= 0)
        {
            break;
        }
        memmove((char *)v->data + (k * v->size), (char *)v->data + (j * v->size), v->size);
        k = j;
    }
    memmove((char *)v->data + (k * v->size), temp, v->size);
}
void heapsort_dynvec(dynvec *v, int (*cmp)(const void *, const void *))
{
    for (int k = v->length / 2 - 1; k >= 0; k--)
    {
        void *temp = malloc(v->size);
        if (!temp) return;
        memcpy(temp, (char *)v->data + k * v->size, v->size);
        movedown(v, k, temp, v->length, cmp);
        free(temp);
    }
    
    void *temp = malloc(v->size);
    if (!temp)
    {
        return;
    }
    for (int k = v->length - 1; k >= 1; k--)
    {
        memcpy(temp, (char *)v->data + k * v->size, v->size);   
        memmove((char *)v->data , (char *)v->data+k*v->size, v->size);     
        memmove((char *)v->data + k * v->size, temp, v->size);    
        movedown(v, 0, temp, k, cmp); 
        
    }
    free(temp);
}

dynvec *dynvec_filter(dynvec *v, bool (*predicate)(void *))
{
    dynvec *temp = dynvec_create(sizeof(v->size));
    if (!v || !predicate)
    {
        return false;
    }
    for (size_t i = 0; i < v->length; i++)
    {
        if (predicate((char *)v->data + (i * v->size)) == 0)
        {
            dynvec_push(temp, (char *)v->data + i * v->size);
        }
    }
    return temp;
}

size_t dynvec_exists_index(dynvec *v, bool (*predicate)(void *))
{

    if (!v || !predicate)
    {
        return false;
    }
    for (size_t i = 0; i < v->length; i++)
    {
        if (predicate((char *)v->data + (i * v->size)) == 0)
        {
            return i;
        }
    }
    return -1;
}
bool dynvec_exists(dynvec *v, bool (*predicate)(void *))
{
    int verifica;
    if (!v || !predicate)
    {
        return false;
    }
    for (size_t i = 0; i < v->length; i++)
    {
        if (predicate((char *)v->data + (i * v->size)) == 0)
        {
            verifica = 1;
        }
    }
    if (verifica == 1)
    {
        return true;
    }
    return false;
}
bool dynvec_forall(dynvec *v, bool (*predicate)(void *))
{
    if (!v || !predicate)
    {
        return false;
    }
    for (size_t i = 0; i < v->length; i++)
    {
        if (predicate((char *)v->data + (i * v->size)) != 0)
        {
            return false;
        }
    }
    return true;
}
bool dynvec_contains(dynvec *v, void *elem, int (*cmp)(const void *, const void *))
{
    if (!v || !cmp)
    {
        return false;
    }
    for (size_t i = 0; i < v->length; i++)
    {
        if (cmp((char *)v->data + (i * v->size), elem) == 0)
        {
            return true;
        }
    }
    return false;
}

void *lsreach(dynvec *v, void *chave, int (*cmp)(const void *, const void *))
{
    if (!v || !cmp)
    {
        return;
    }
    for (size_t i = 0; i < v->length; i++)
    {
        if (cmp((char *)v->data + (i * v->size), chave) == 0)
        {
            printf("Encontrado no indice : \n", i);
        }
    }
    printf("Não encontrado\n");
    return NULL;
}

void *bsearch(dynvec *v, void *chave, int (*cmp)(const void *, const void *))
{
    if (!v || !cmp)
    {
        return NULL;
    }
    size_t left = 0, right = v->length - 1, meio = left + (right - left) / 2;
    while (left <= right)
    {
        if (cmp((char *)v->data + (meio * v->size), chave) == 0)
        {
            printf("Encontrado no indice : %zu\n", meio);
            return (char *)v->data + (meio * v->size);
        }
        if (cmp((char *)v->data + (meio * v->size), chave) < 0)
        {
            left = meio + 1;
        }
        else
        {
            right = meio - 1;
        }
        meio = left + (right - left) / 2;
    }
    printf("Não encontrado\n");
    return NULL;
}
void dynvec_map(dynvec *v, void (*processo)(void *))
{
    if (!v || !processo)
        return;
    for (size_t i = 0; i < v->length; i++)
    {
        processo((char *)v->data + (i * v->size));
    }
}
void resize(dynvec *v)
{
    if (!v->data)
    {
        v->capacity = 4;
        v->data = malloc(v->capacity * v->size);
        if (!v->data)
        {
#ifdef DEBUG_ON
            perror("Push. Falha ao realocar memória");
#endif
            return;
        }
    }
    if (v->length >= v->capacity)
    {
        size_t new_capacity = v->capacity * 2;

        void *new_data = realloc(v->data, v->size * new_capacity);

        if (!new_data)
        {
#ifdef DEBUG_ON
            perror("Push. Falha ao realocar memória");
#endif
            return;
        }
        v->capacity = new_capacity;
        v->data = new_data;
    }
    if (v->length <= v->capacity / 4 && v->capacity > 4)
    {
        size_t new_capacity = v->capacity / 2;
        if (new_capacity < 4)
            new_capacity = 4;
        if (new_capacity >= 4)
        {
            void *new_data = realloc(v->data, new_capacity * v->size);
            if (!new_data)
                return;

            v->data = new_data;
            v->capacity = new_capacity;
        }
    }
}
void dynvec_empty(dynvec *v)
{
    if (v != NULL)
    {
        free(v->data);
        v->data = NULL;
    }
}

size_t dynvec_capacity(dynvec *v)
{
    return v->capacity;
}

size_t dynvec_length(dynvec *v)
{
    if (v == 0)
    {
        return 0;
    }
    return v->length;
}

void dynvec_push(dynvec *v, void *x)
{
    if (v->length +1<= v->capacity)
    {
        memcpy((char *)v->data + v->length * v->size, x, v->size);
        v->length++;
    }
    else
    {
        resize(v);
        memcpy((char *)v->data + v->length * v->size, x, v->size);
        v->length++;
    }
    
}

void *dynvec_pop(dynvec *v)
{

    if (v->length == 0)
    {
        return NULL;
    }
    dynvec_delete(v, v->length - 1);
}

int *dynvec_top(dynvec *v)
{
    if (v == NULL)
    {
        return 0;
    }
    return (int *)((char *)v->data + (v->length - 1) * v->size);
}

void dynvec_set(dynvec *v, size_t index, void *x)
{
    memmove((char *)v->data + index * v->size, x, v->size);
}

void *dynvec_get(dynvec *v, size_t index)
{
    if (index >= v->length)
    {
        errno = ERANGE;
    }

    return ((char *)v->data + index * v->size);
}

void dynvec_insert(dynvec *v, size_t index, void *x)
{
    if (index >= v->length)
    {
        errno = ERANGE;
    }

    memmove((char *)v->data + (index + 1) * v->size, (char *)v->data + (index)*v->size, (v->length - index) * v->size);

    memcpy((char *)v->data + index * v->size, x, v->size);
    v->length++;
}
void dynvec_delete(dynvec *v, size_t index)
{
    if (index >= v->length)
    {
        errno = ERANGE;
    }
    if (index < v->length )
    {
        memmove((char *)v->data + (index)*v->size, (char *)v->data + (index + 1) * v->size, (v->length - index - 1) * v->size);
        
    }
    
        v->length--;
        resize(v);

}

void dynvec_free(dynvec *v)
{
    if (v != NULL)
    {
        free(v->data);
    }
    v->data = NULL;
    v->capacity = DYNVEC_INIT_CAPACITY;
    v->length = 0;
    free(v);
    return;
}