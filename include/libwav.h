#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#ifndef _LIBWAV_H
#define _LIBWAV_H

#include <stdint.h>

struct WAVfmtData {
  uint16_t wFormatTag;
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};

struct WAVextensibleFmtData {
  uint16_t wValidBitsPerSample;
  uint32_t dwChannelMask;
  char SubFormat[16]; // GUID
};

struct WAVdata {
  char id[4]; // Exactly 4 characters, no null terminator
  uint32_t size;
  void *data; // Pointer to the data
};

struct WavFile {
  struct WAVfmtData fmt_data;
  uint32_t cbSize;
  struct WAVextensibleFmtData ext_fmt_data;
  struct WAVdata data_chunk;
};

int parse_wav(const char *filename, struct WavFile *wav_file_data);

#endif // _LIBWAV_H

#ifdef __cplusplus
}
#endif