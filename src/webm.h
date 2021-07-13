#ifndef WEBM_H
#define WEBM_H

#include <stdio.h>

extern void AppendWEBMDuration(char* pFilePath, int iDuration, size_t iDurationByte);
extern void ProcessWEBM(char* pFilePath, int* pDuration);

#endif
