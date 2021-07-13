#include "shared.h"

void GetOffsetData(FILE* pFile, unsigned long iOffset, void* pBuffer, size_t BufferSize)
{
	fseek(pFile, iOffset, SEEK_SET);
	fread(pBuffer, BufferSize, 1, pFile);
}