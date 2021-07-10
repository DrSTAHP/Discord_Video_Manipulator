#include "shared.h"

void GetOffsetData(FILE* pFile, unsigned long iOffset, void* pBuffer, size_t BufferSize)
{
	fseek(pFile, iOffset, SEEK_SET);
	fread(pBuffer, BufferSize, 1, pFile);
}

//This is needed due to the fact that
//MPEG-4 and WEBM headers are big-endian and modern CPUs are little-endian.
//It essentially tricks the CPU to "invert" little-endianned bytes, in effect
//creating a big-endian. 
void split_bytes(int iNumber, uint8_t* pBuffer)
{
	pBuffer[0] = (uint8_t)(iNumber >> 3 * (BYTE));
	pBuffer[1] = (uint8_t)(iNumber >> 2 * (BYTE));
	pBuffer[2] = (uint8_t)(iNumber >> (BYTE));
	pBuffer[3] = (uint8_t)(iNumber >> 0);
}