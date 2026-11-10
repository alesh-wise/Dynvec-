# dynvec — Vetor dinâmico genérico em C

<p>
  <img src="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white" alt="C"/>
</p>

O **dynvec** é uma estrutura de dados dinâmica em C, desenvolvida ao longo da disciplina de **Laboratório de Programação** da Universidade do Algarve. É um vetor que cresce e encolhe automaticamente e guarda elementos de **qualquer tipo** (`int`, `double`, `struct`…), com funções de inserção, remoção, pesquisa, ordenação e funções de ordem superior (`map`, `filter`, `fold`).

## Funcionalidades

- **Genérico:** guarda os dados num `void *` e trabalha com o tamanho de cada elemento (`size`), como o `qsort` da biblioteca standard.
- **Gestão de memória automática:** começa com capacidade 4, **duplica** quando fica cheio e **reduz para metade** quando fica a ¼ da capacidade, para não desperdiçar memória.
- **Comparadores e predicados como ponteiros para funções:** o utilizador define como se comparam e filtram os elementos.
- **Vários algoritmos de ordenação** implementados de raiz, para comparar abordagens.
- **Erros sinalizados com `errno`** (`ENOMEM`, `ERANGE`) e mensagens de debug opcionais com `-DDEBUG_ON`.

## Estrutura

```c
typedef struct Dynvec {
    void  *data;      // bloco contíguo com os elementos
    size_t size;      // tamanho de cada elemento (bytes)
    size_t capacity;  // número de elementos que cabem
    size_t length;    // número de elementos guardados
} dynvec;
```

## API

### Criação e memória

| Função | Descrição |
| --- | --- |
| `dynvec *dynvec_create(size_t size)` | Cria um vetor vazio para elementos de `size` bytes |
| `void dynvec_free(dynvec *v)` | Liberta o vetor e os dados |
| `void dynvec_empty(dynvec *v)` | Liberta só os dados |
| `size_t dynvec_length(dynvec *v)` | Número de elementos |
| `size_t dynvec_capacity(dynvec *v)` | Capacidade atual |
| `void resize(dynvec *v)` | Ajusta a capacidade (chamada automaticamente) |

### Acesso e modificação

| Função | Descrição |
| --- | --- |
| `void dynvec_push(dynvec *v, void *x)` | Acrescenta um elemento no fim |
| `void *dynvec_pop(dynvec *v)` | Remove o último elemento |
| `int *dynvec_top(dynvec *v)` | Ponteiro para o último elemento |
| `void *dynvec_get(dynvec *v, size_t i)` | Ponteiro para o elemento na posição `i` |
| `void dynvec_set(dynvec *v, size_t i, void *x)` | Substitui o elemento na posição `i` |
| `void dynvec_insert(dynvec *v, size_t i, void *x)` | Insere na posição `i`, deslocando os seguintes |
| `void dynvec_delete(dynvec *v, size_t i)` | Remove a posição `i`, deslocando os seguintes |

### Pesquisa

| Função | Descrição |
| --- | --- |
| `bool dynvec_contains(v, elem, cmp)` | Indica se o elemento existe |
| `size_t dynvec_index(v, elem, cmp)` | Índice da primeira ocorrência (ou `-1`) |
| `void *lsreach(v, chave, cmp)` | Pesquisa linear, O(n) |
| `void *bsearch(v, chave, cmp)` | Pesquisa binária, O(log n), com o vetor ordenado |

### Ordem superior

| Função | Descrição |
| --- | --- |
| `void dynvec_map(v, f)` | Aplica `f` a cada elemento (no lugar) |
| `dynvec *dynvec_filter(v, pred)` | Novo vetor com os elementos que cumprem `pred` |
| `void dynvec_fold_left(v, acc, f)` | Acumula os elementos da esquerda para a direita |
| `bool dynvec_exists(v, pred)` | Algum elemento cumpre `pred`? |
| `size_t dynvec_exists_index(v, pred)` | Índice do primeiro que cumpre `pred` |
| `bool dynvec_forall(v, pred)` | Todos os elementos cumprem `pred`? |

### Ordenação

| Função | Algoritmo | Complexidade média |
| --- | --- | --- |
| `insertion_sort_dynvec(v, cmp)` | Insertion sort | O(n²) |
| `mergesort_dynvec(v, cmp, 0, n)` | Merge sort (intervalo `[left, right)`) | O(n log n), estável |
| `heapsort_dynvec(v, cmp)` | Heap sort (max-heap) | O(n log n), no lugar |
| `quicksort(v, cmp)` | Quicksort com pivô aleatório | O(n log n) |
| `quicksort_dynvec_three_way(v, cmp, 0, n - 1)` | Quicksort com partição em 3 (Dijkstra), bom com muitos repetidos | O(n log n) |

Também estão disponíveis as partições usadas internamente: `partition` (pivô aleatório), `partition_lomuto` e `partition_media_of_three` (mediana de três).

## Como usar

### Exemplo

```c
#include <stdio.h>
#include "dynvec.h"

int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

void dobrar(void *e) { *(int *)e *= 2; }

void somar(void *acc, void *e) { *(int *)acc += *(int *)e; }

int main(void) {
    dynvec *v = dynvec_create(sizeof(int));

    int valores[] = {7, 3, 9, 1, 5};
    for (int i = 0; i < 5; i++)
        dynvec_push(v, &valores[i]);

    quicksort(v, cmp_int);          // 1 3 5 7 9
    dynvec_map(v, dobrar);          // 2 6 10 14 18

    int soma = 0;
    dynvec_fold_left(v, &soma, somar);

    for (size_t i = 0; i < dynvec_length(v); i++)
        printf("%d ", *(int *)dynvec_get(v, i));
    printf("\nsoma = %d\n", soma);  // soma = 50

    dynvec_free(v);
    return 0;
}
```

### Compilar

Com o exemplo acima guardado em `main.c`, na mesma pasta que `dynvec.c` e `dynvec.h`:

```bash
gcc -Wall -std=c11 -o main main.c dynvec.c
./main
```

Para ver as mensagens de erro de alocação:

```bash
gcc -Wall -std=c11 -DDEBUG_ON -o main main.c dynvec.c
```

## Estado

O projeto foi escrito em 2025 durante a cadeira e ficou como estava no fim do semestre. Com compiladores recentes (GCC 14 ou superior) é preciso fazer pequenos ajustes para compilar:

- a função `bsearch` tem o mesmo nome que a da `<stdlib.h>`, com outra assinatura (renomear, por exemplo, para `dynvec_bsearch`);
- falta `#include <stdio.h>` em `dynvec.c`, por causa do `printf` das funções de pesquisa;
- `partition_three_way` e `lsreach` têm `return;` sem valor em funções que devolvem um valor.

## Autor

**Alexandre Guerreiro**: estudante na Universidade do Algarve ([@alesh-wise](https://github.com/alesh-wise)).
