#ifndef MEM_H_
#define MEM_H_

/**
 * Copyright (c) 2013, rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 * change for my projects Luis Santos aka DJOKER credits to rxi :)
 */
#include <stdio.h>

#define mem_malloc(sz)        _mem_alloc(sz, 0, __FILE__, __LINE__)
#define mem_calloc(num, sz)   _mem_alloc((num) * (sz), 1, __FILE__, __LINE__)
#define mem_realloc(ptr, sz)  _mem_realloc(ptr, sz, __FILE__, __LINE__)
#define mem_free(ptr)         _mem_free(ptr, __FILE__, __LINE__)
#define mem_size(ptr)         _mem_size(ptr, __FILE__, __LINE__)

void   *_mem_alloc(size_t, int, const char*, unsigned);
void   *_mem_realloc(void*, size_t, const char*, unsigned);
void    _mem_free(void*, const char*, unsigned);
size_t  _mem_size(void*, const char*, unsigned);

void    mem_dump(FILE*);
size_t  mem_usage(void);
int     mem_has(void *ptr);

#endif

#ifdef MEMORY_IMPLEMENT


#include <stdio.h>
#include <stdlib.h>
#include <string.h>




typedef struct mem_node_t 
{
  struct mem_node_t *prev, *next;
  const char *file;
  size_t line;
  size_t size;

} mem_node_t;


mem_node_t *mem_head;



int _mem_has_node(mem_node_t *n)
 {
  mem_node_t *node = mem_head;
  while (node != NULL) 
  {
    if (node == n) return 1;
    node = node->next;
  }
  return 0;
}



void _mem_abort(void)
{

  abort();
}



void *_mem_alloc(size_t sz, int zeroset, const char *file, unsigned line)
 {
  mem_node_t *node = NULL;
  
  if (zeroset) 
  {
    node = calloc(sizeof(*node) + sz, 1);
  } else 
  {
    node = malloc(sizeof(*node) + sz);
    if (node != NULL) 
    {
      memset(node, 0, sizeof(*node));
    }
  }

  if (node == NULL) 
  {

    fprintf(stderr, "Couldn't allocate: %s, line %u\n", file, line);
    _mem_abort();

  }

  node->line = line;
  node->file = file;
  node->size = sz;


  if (mem_head)
   {
    mem_head->prev = node;
    node->next = mem_head;
  }
  mem_head = node;

  return (char*)node + sizeof(*node);
}



void *_mem_realloc(void *ptr, size_t sz, const char *file, unsigned line)
 {
  mem_node_t *node = (mem_node_t*)((char*)ptr - sizeof(*node));
  mem_node_t *old_node = node;

  if (ptr == NULL) return _mem_alloc(sz, 0, file, line);

  if (!_mem_has_node(node))
   {
    fprintf(stderr, "Bad realloc: %p %s, line %u\n", ptr, file, line);
    _mem_abort();
  }


  node = realloc(node, sizeof(*node) + sz);

  if (node == NULL) 
  {

    fprintf(stderr, "Couldn't reallocate: %s, line %u\n", file, line);
    _mem_abort();

  }

  node->size = sz;
  if (mem_head == old_node) mem_head = node;
  if (node->prev) node->prev->next = node;
  if (node->next) node->next->prev = node;

  return (char*)node + sizeof(*node);
}



void _mem_free(void *ptr, const char *file, unsigned line)
 {
  mem_node_t *node = (mem_node_t*)((char*)ptr - sizeof(*node));

  if (ptr == NULL) return;

  if (!_mem_has_node(node))
   {
     fprintf(stderr, "Bad free: %p %s, line %u\n", ptr, file, line);
    _mem_abort();
  }


  if (node == mem_head) mem_head = node->next;
  if (node->prev) node->prev->next = node->next;
  if (node->next) node->next->prev = node->prev;

  free(node);
  node=NULL;
}



void mem_dump(FILE *fp) 
{
  mem_node_t *node = mem_head;
  size_t total = 0;

  if (!fp) fp = stdout;

  while (node != NULL)
   {
    fprintf(fp, "Unfreed: %p %s, line %lu (%lu bytes)\n", 
            (char*)node + sizeof(*node), node->file,
            (unsigned long)node->line, (unsigned long)node->size);

    total += node->size;
    node = node->next;
  }

  fprintf(fp, "Total unfreed: %lu bytes\n", (unsigned long)total);
}



size_t _mem_size(void *ptr, const char* file, unsigned line)
 {
  mem_node_t *node = (mem_node_t*)((char*)ptr - sizeof(*node));


  if (!_mem_has_node(node)) 
  {
    fprintf(stderr, "Bad pointer: %p %s, line %u\n", ptr, file, line);
    _mem_abort();
  }


  return node->size;
}



size_t mem_usage(void) 
{
  mem_node_t *node = mem_head;
  size_t total = 0;

  while (node != NULL)
  {
    total += node->size;
    node = node->next;
  } 

  return total;
}



int mem_has(void *ptr)
{
  mem_node_t *node = (mem_node_t*)((char*)ptr - sizeof(*node));
  return _mem_has_node(node);
}

#endif
