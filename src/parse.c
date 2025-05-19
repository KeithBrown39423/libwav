#include "dict.h"
#include "libwav.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_wav(const char *filename, struct WavFile *wav_file_data) {
  // Open the WAV file
  FILE *wav_file = malloc(sizeof(FILE));
  fopen_s(&wav_file, filename, "rb");

  fseek(wav_file, 0, SEEK_END);
  long file_size = ftell(wav_file);
  fseek(wav_file, 0, SEEK_SET);

  // Read the WAV header
  char riff[4];
  fread(&riff, sizeof(char), 4, wav_file);

  uint32_t riff_chunk_size;
  fread(&riff_chunk_size, sizeof(uint32_t), 1, wav_file);

  char wave[4];
  fread(wave, sizeof(char), 4, wav_file);

  LookupTable table;
  init_table(&table, 4);

  while (ftell(wav_file) < file_size) {
    char chunk_id[4];
    fread(&chunk_id, sizeof(char), 4, wav_file);

    uint32_t chunk_size;
    fread(&chunk_size, sizeof(uint32_t), 1, wav_file);

    add_entry(&table, chunk_id,
              ftell(wav_file) - 8); // Offset starts at the Chunk ID

    fseek(wav_file, chunk_size, SEEK_CUR);
  }

  struct WAVfmtData wav_fmt_data = {0};
  uint32_t cbSize = 0;
  struct WAVextensibleFmtData wav_ext_fmt_data = {0};

  Entry *fmt_entry = find_entry(&table, "fmt ");
  if (fmt_entry) {
    fseek(wav_file, fmt_entry->offset, SEEK_SET);
    char fmt_id[4];
    fread(&fmt_id, sizeof(char), 4, wav_file);
    uint32_t fmt_size;
    fread(&fmt_size, sizeof(uint32_t), 1, wav_file);

    fread(&wav_fmt_data, sizeof(struct WAVfmtData), 1, wav_file);

    if (fmt_size == 18) {
      fread(&cbSize, sizeof(uint32_t), 1, wav_file);
    }

    if (cbSize == 22) {
      fread(&wav_ext_fmt_data, sizeof(struct WAVextensibleFmtData), 1,
            wav_file);
    }
  } else {
    fprintf(stderr, "No 'fmt ' chunk found.\n");
  }

  struct WAVdata wav_data = {0};
  Entry *data_entry = find_entry(&table, "data");
  if (data_entry) {
    fseek(wav_file, data_entry->offset, SEEK_SET);
    fread(&wav_data.id, sizeof(char), 4, wav_file);
    fread(&wav_data.size, sizeof(uint32_t), 1, wav_file);

    // Read the data chunk
    wav_data.data = malloc(wav_data.size);

    fread(wav_data.data, sizeof(uint8_t), wav_data.size, wav_file);
  } else {
    fprintf(stderr, "No 'data' chunk found.\n");
  }

  wav_file_data->fmt_data = wav_fmt_data;
  wav_file_data->cbSize = cbSize;
  wav_file_data->ext_fmt_data = wav_ext_fmt_data;
  wav_file_data->data_chunk = wav_data;

  free_table(&table);
  fclose(wav_file);
  free(wav_file);

  return 0;
}
