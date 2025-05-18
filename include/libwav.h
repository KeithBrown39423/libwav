#pragma once
#ifndef _LIBWAV_H
#define _LIBWAV_H

#include <stdint.h>

struct WAVData {
  uint16_t wFormatTag;
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
  uint16_t cbSize;
  uint16_t wValidBitsPerSample;
  uint32_t dwChannelMask;
  void *data;
  uint32_t data_size;
};

int parse_wav(const char *filename, struct WAVData *wav_data);

#endif // _LIBWAV_H
