#pragma once
#ifndef _DICT_H
#define _DICT_H

#include <stddef.h>

typedef struct {
  char id[4]; // Exactly 4 characters, no null terminator
  size_t offset;
} Entry;

typedef struct {
  Entry *entries;
  size_t size;
  size_t capacity;
} LookupTable;

void init_table(LookupTable *table, size_t initial_capacity);
void add_entry(LookupTable *table, const char *id, size_t offset);
Entry *find_entry(const LookupTable *table, const char *id);
void print_table(const LookupTable *table);
void free_table(LookupTable *table);

#endif // _DICT_H
