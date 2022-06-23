# Real malloc challenge!

## Instruction

Your task is implement a better malloc logic in [malloc.c](malloc.c) to improve the speed and memory usage.

## How to build & run a benchmark

```
# build
make
# run a benchmark
make run
```

If the commands above don't work, you can build and run the challenge directly by running:

```
gcc -Wall -O3 -lm -o malloc_challenge.bin main.c malloc.c simple_malloc.c
./malloc_challenge.bin
```

## What I do 

### best fit 

```c

  // best fit
  // Select the smallest difference
my_metadata_t *best_prev = NULL;
my_metadata_t *best_metadata = NULL;
  while (metadata)
  {
    if (size <= metadata->size){
      if (!best_metadata ||  best_metadata->size > metadata->size)
      {
        best_prev=prev;
        best_metadata = metadata;
      }
    }
    prev = metadata;
    metadata = metadata->next;
  }

  metadata = best_metadata;
  prev = best_prev;
```

### free list bin

![image](https://user-images.githubusercontent.com/92320824/175279484-744c8f96-b5d9-4b10-a012-cf3144fc33ed.png)
reference [hikalium's slide](https://docs.google.com/presentation/d/1y0rgZJDd7jqumQ2XbKcFxMzZhAbtNbml0SXwSs6QOcg/edit?pli=1#slide=id.g13622c17fa3_2_19)

実際にこのように実装できているのかよくわかってない（処理時間が特段速くなった訳ではなかった）

## Acknowledgement

This work is based on [xharaken's malloc_challenge.c](https://github.com/xharaken/step2/blob/master/malloc_challenge.c). Thank you haraken-san!
