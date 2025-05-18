#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[4]; // Exactly 4 characters, no null terminator
  size_t offset;
} Entry;

typedef struct {
  Entry *entries;
  size_t size;
  size_t capacity;
} LookupTable;

void init_table(LookupTable *table, size_t initial_capacity) {
  table->entries = malloc(initial_capacity * sizeof(Entry));
  table->size = 0;
  table->capacity = initial_capacity;
}

void add_entry(LookupTable *table, const char *id, size_t offset) {
  if (table->size == table->capacity) {
    size_t new_capacity = table->capacity * 2;
    Entry *new_entries = realloc(table->entries, new_capacity * sizeof(Entry));
    if (!new_entries) {
      perror("Realloc failed");
      exit(EXIT_FAILURE);
    }
    table->entries = new_entries;
    table->capacity = new_capacity;
  }

  memcpy(table->entries[table->size].id, id, 4); // Copy exactly 4 bytes
  table->entries[table->size].offset = offset;
  table->size++;
}

Entry *find_entry(const LookupTable *table, const char *id) {
  for (size_t i = 0; i < table->size; i++) {
    if (memcmp(table->entries[i].id, id, 4) == 0) {
      return &table->entries[i];
    }
  }
  return NULL;
}

void print_table(const LookupTable *table) {
  for (size_t i = 0; i < table->size; i++) {
    printf("Entry %zu: ID = %.4s", i, table->entries[i].id);
    printf(", Offset = %zu\n", table->entries[i].offset);
  }
}

void free_table(LookupTable *table) {
  free(table->entries);
  table->entries = NULL;
  table->size = 0;
  table->capacity = 0;
}
