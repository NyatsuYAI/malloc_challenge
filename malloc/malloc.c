//
// >>>> malloc challenge! <<<<
//
// Your task is to improve utilization and speed of the following malloc
// implementation.
// Initial implementation is the same as the one implemented in simple_malloc.c.
// For the detailed explanation, please refer to simple_malloc.c.

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//
// Interfaces to get memory pages from OS
//

void *mmap_from_system(size_t size);
void munmap_to_system(void *ptr, size_t size);

//
// Struct definitions
//
// metadataにBinsize覚えさせよう

typedef struct my_metadata_t {
  size_t size;
  int bin_size;
  struct my_metadata_t *next;
} my_metadata_t;

typedef struct my_heap_t {
  my_metadata_t *free_head;
  my_metadata_t dummy;
} my_heap_t;


//
// Static variables (DO NOT ADD ANOTHER STATIC VARIABLES!)
//
my_heap_t my_heap[12];
//
// Helper functions (feel free to add/remove/edit!)
//

int exponentiation_list[13] = {0,2,4,8,16,32,64,128,256,512,1024,2048,4096};


    // 遅いから小さいサイズを細かくチェック
int define_bin_from_size(size_t size)
{
  for (int i = 0;i < 12; i++){
    if (exponentiation_list[i] <= size && size < exponentiation_list[i+1])
    {
      return i;
    }
  }
  return 11;
}

void my_add_to_free_list(my_metadata_t *metadata) {
  assert(!metadata->next);
  metadata->next = my_heap[metadata->bin_size].free_head;
  my_heap[metadata->bin_size].free_head = metadata;
}

void my_remove_from_free_list(my_metadata_t *metadata, my_metadata_t *prev, my_heap_t *my_heap)
{
  if (prev) {
    prev->next = metadata->next;
  } else {
    my_heap[metadata->bin_size].free_head = metadata->next;
  }
  metadata->next = NULL;
}

//
// Interfaces of malloc (DO NOT RENAME FOLLOWING FUNCTIONS!)
//

// This is called at the beginning of each challenge.
void my_initialize() {
  for(int i=0;i<12;i++){
    my_heap[i].free_head = &my_heap[i].dummy;
    my_heap[i].dummy.size = 0;
    my_heap[i].dummy.bin_size = i;
    my_heap[i].dummy.next = NULL;
    printf("inited my_heap %d\n", my_heap[i].dummy.bin_size);
    }
}

// my_malloc() is called every time an object is allocated.
// |size| is guaranteed to be a multiple of 8 bytes and meets 8 <= |size| <=
// 4000. You are not allowed to use any library functions other than
// mmap_from_system() / munmap_to_system().
void *my_malloc(size_t size) {
  int bin_size = define_bin_from_size(size);
  my_metadata_t *metadata = my_heap[bin_size].free_head;
  my_metadata_t *prev = NULL;

  my_metadata_t *best_prev = NULL;
  my_metadata_t *best_metadata = NULL;

  printf("want bin is :%d and size: %zu\n", bin_size, size);
  // best fit
  // Select the smallest difference

  while (metadata)
  {
    if (size <= metadata->size){
        printf("wo");
      if (!best_metadata ||  best_metadata->size > metadata->size)
      {
        best_prev=prev;
        best_metadata = metadata;
      }
    }
    prev = metadata;
    metadata = metadata->next;
    printf("=========Change==========");
    if (!metadata && metadata->bin_size < 12){
      bin_size++;
      metadata = my_heap[bin_size].free_head;
      prev = NULL;
      printf("up size :%d",bin_size);
    }
  }

  // now, metadata points to the first free slot
  // and prev is the previous entry.

  if (!metadata) {
    // There was no free slot available. We need to request a new memory region
    // from the system by calling mmap_from_system().
    //
    //     | metadata | free slot |
    //     ^
    //     metadata
    //     <---------------------->
    //            buffer_size
    size_t buffer_size = 4096;
    my_metadata_t *metadata = (my_metadata_t *)mmap_from_system(buffer_size);
    metadata->size = buffer_size - sizeof(my_metadata_t);
    metadata->bin_size = define_bin_from_size(metadata->size);
    metadata->next = NULL;
    // Add the memory region to the free list.
    my_add_to_free_list(metadata);
    // Now, try my_malloc() again. This should succeed.
    return my_malloc(size);
  }


  printf("best mata: %d\n", best_metadata->bin_size);
  // |ptr| is the beginning of the allocated object.
  //
  // ... | metadata | object | ...
  //     ^          ^
  //     metadata   ptr
  void *ptr = best_metadata + 1;
  size_t remaining_size = best_metadata->size - size;
  best_metadata->size = size;
  best_metadata->bin_size = define_bin_from_size(best_metadata->size);
  // Remove the free slot from the free list.
  my_remove_from_free_list(best_metadata, best_prev, my_heap);

  if (remaining_size > sizeof(my_metadata_t)) {
    // Create a new metadata for the remaining free slot.
    //
    // ... | metadata | object | metadata | free slot | ...
    //     ^          ^        ^
    //     metadata   ptr      new_metadata
    //                 <------><---------------------->
    //                   size       remaining size
    my_metadata_t *new_metadata = (my_metadata_t *)((char *)ptr + size);
    new_metadata->size = remaining_size - sizeof(my_metadata_t);
    new_metadata->bin_size = define_bin_from_size(new_metadata->size);
    new_metadata->next = NULL;
    // Add the remaining free slot to the free list.
    my_add_to_free_list(new_metadata);
  }
  return ptr;
}

// This is called every time an object is freed.  You are not allowed to
// use any library functions other than mmap_from_system / munmap_to_system.
void my_free(void *ptr) {
  // Look up the metadata. The metadata is placed just prior to the object.
  //
  // ... | metadata | object | ...
  //     ^          ^
  //     metadata   ptr
  my_metadata_t *metadata = (my_metadata_t *)ptr - 1;
  // Add the free slot to the free list.
  my_add_to_free_list(metadata);
}

// This is called at the end of each challenge.
void my_finalize() {
  // Nothing is here for now.
  // feel free to add something if you want!
}

void test() {
  // Implement here!
  assert(1 == 1); /* 1 is 1. That's always true! (You can remove this.) */
}
