#ifndef WEBM_H
#define WEBM_H

#include <stdio.h>
#include <stdint.h>

extern void AppendWEBMDuration(char* pFilePath, int16_t iDuration, size_t iDurationByte);
extern void ProcessWEBM(char* pFilePath, int16_t* pDuration);

#endif
