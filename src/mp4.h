#ifndef MP4_H
#define MP4_H

#include <stdio.h>

#define MVHD_TIME_OFFSET 12 //12 bytes

extern size_t GetmvhdOffset(char* pFilePath);
extern void AppendMP4Duration(char* pFilePath, int iDuration, size_t iMVHD);
extern void ProcessMP4(char* pFilePath, int* pDuration);

#endif