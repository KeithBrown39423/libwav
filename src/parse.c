#include "libwav.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int parse_wav(const char *filename, struct WAVData *wav_data) {
  // Open the WAV file
  FILE *wav_file = {0};
  fopen_s(&wav_file, filename, "rb");

  if (wav_file == NULL) {
    perror("Failed to open WAV file");
    return 1;
  }

  // Read the WAV header
  char riff[4];
  fread(riff, sizeof(char), 4, wav_file);
  if (strncmp(riff, "RIFF", 4) != 0)
    goto INVALID_FILE;

  uint32_t riff_chunk_size;
  fread(&riff_chunk_size, sizeof(uint32_t), 1, wav_file);

  char wave[4];
  fread(wave, sizeof(char), 4, wav_file);
  if (strncmp(wave, "WAVE", 4) != 0)
    goto INVALID_FILE;

  char fmt[4];
  fread(fmt, sizeof(char), 4, wav_file);
  if (strncmp(fmt, "fmt ", 4) != 0)
    goto INVALID_FILE;

  uint32_t fmt_chunk_size;
  fread(&fmt_chunk_size, sizeof(uint32_t), 1, wav_file);
  if (fmt_chunk_size < 16)
    goto INVALID_FILE;

  fread(&wav_data->wFormatTag, sizeof(uint16_t), 1, wav_file);
  fread(&wav_data->nChannels, sizeof(uint16_t), 1, wav_file);
  fread(&wav_data->nSamplesPerSec, sizeof(uint32_t), 1, wav_file);
  fread(&wav_data->nAvgBytesPerSec, sizeof(uint32_t), 1, wav_file);
  fread(&wav_data->nBlockAlign, sizeof(uint16_t), 1, wav_file);
  fread(&wav_data->wBitsPerSample, sizeof(uint16_t), 1, wav_file);
  if (fmt_chunk_size > 16) {
    fread(&wav_data->cbSize, sizeof(uint16_t), 1, wav_file);
    if (fmt_chunk_size > 18) {
      fread(&wav_data->wValidBitsPerSample, sizeof(uint16_t), 1, wav_file);
      fread(&wav_data->dwChannelMask, sizeof(uint32_t), 1, wav_file);
      fseek(wav_file, 16, SEEK_CUR); // Skip subformat
    }
  }

  char chunk_id[4];
  // Find the "data" chunk
  while (fread(chunk_id, sizeof(char), 4, wav_file) == 4) {
    if (strncmp(chunk_id, "data", 4) == 0)
      break;
    uint32_t chunk_size;
    fread(&chunk_size, sizeof(uint32_t), 1, wav_file);
    fseek(wav_file, chunk_size, SEEK_CUR);
  }

  fread(&wav_data->data_size, sizeof(uint32_t), 1, wav_file);

  wav_data->data = malloc(wav_data->data_size);
  if (wav_data->data == NULL) {
    perror("Failed to allocate memory for WAV data");
    fclose(wav_file);
    return 1;
  }
  fread(wav_data->data, sizeof(char), wav_data->data_size, wav_file);
  if (ferror(wav_file)) {
    perror("Failed to read WAV data");
    free(wav_data->data);
    fclose(wav_file);
    return 1;
  }

  // VALID EXIT
  fclose(wav_file);
  return 0;
INVALID_FILE:
  fprintf(stderr, "Invalid WAV file format\n");
  fclose(wav_file);
  return 1;
}
