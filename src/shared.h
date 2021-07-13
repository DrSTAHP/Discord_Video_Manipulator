#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdint.h>
#ifdef _WIN32
#include <winsock2.h>
#elif __linux__
#include <arpa/inet.h>
#endif

#define BYTE 8 //8 bits
#define EIGHT_BITS 1 //1 byte

#define TRUE 1
#define FALSE 0

extern void GetOffsetData(FILE* pFile, unsigned long iOffset, void* pBuffer, size_t BufferSize);

#endif