#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdint.h>

#define BYTE 8 //8 bits
#define EIGHT_BITS 1 //1 byte

extern void GetOffsetData(FILE* pFile, unsigned long iOffset, void* pBuffer, size_t BufferSize);
extern void split_bytes(int iNumber, uint8_t* pBuffer);

#endif