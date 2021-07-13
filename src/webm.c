#include "webm.h"
#include "shared.h"
#include <stdint.h>
#include <stdio.h>

static uint8_t compare_bytes(uint8_t* pArr1, uint8_t* pArr2, size_t iSize)
{
	for(int i = 0; i < iSize; i++)
	{
		if(pArr1[i] != pArr2[i])
			return FALSE;
	}
	return TRUE;
}

static void shift_bytes(int iNumber, uint8_t* pBuffer, uint8_t iNumofBytes)
{
	uint8_t iBytePush = BYTE * (iNumofBytes - 1);
	
	for(unsigned int i = 0; i < iNumofBytes; i++)
	{
		pBuffer[i] = (uint8_t)(iNumber >> iBytePush);
		iBytePush -= BYTE;
	}
}

static size_t GetDurationOffset(char* pFilePath)
{

	FILE* pFile = NULL;

#ifdef _WIN32
	fopen_s(&pFile, pFilePath, "r");
#elif __linux__
	pFile = fopen(pFilePath, "r");
#endif

	size_t iOffset = 0;

	//Get the file size and reset the pointer.
	fseek(pFile, 0, SEEK_END);
	size_t iFileSize = ftell(pFile);

	fseek(pFile, 0, SEEK_SET);

	const unsigned int iTimeScaleByte = 0x2AD7B1;
	const uint16_t iDurationByte = htons(0x4489);

	//Since there is no 3-byte integer type in C, I've got to create a byte array. Sweet!
	uint8_t iTimeScaleByteArray[3];
	shift_bytes(iTimeScaleByte,iTimeScaleByteArray, 3); //I'm just going to leave it like it is, there will be no need to shift bytes.

	while(iOffset <= iFileSize) //Find TSB offset.
	{
		uint8_t iData[3];
		GetOffsetData(pFile, iOffset,iData,3);

		if(compare_bytes(iData, iTimeScaleByteArray, 3))
		{	
			//printf("FOUND TSB: %li\n",iOffset);	
			break;
		}
		iOffset++;
	}

	while(iOffset <= iFileSize) //Find DB offset.
	{
		uint16_t iData = 0;
		GetOffsetData(pFile,iOffset, &iData, sizeof(uint16_t));

		if(iData == iDurationByte)
		{	
			//printf("FOUND DB %li\n",iOffset);	
			break;
		}
		iOffset++;
	}

	fseek(pFile, 0, SEEK_SET);

	fclose(pFile);
	return iOffset;
}

void AppendWEBMDuration(char* pFilePath, int iDuration,size_t iDurationByte)
{

	FILE* pFile = NULL;

#ifdef _WIN32
	fopen_s(&pFile, pFilePath, "r+");
#elif __linux__
	pFile = fopen(pFilePath, "r+");
#endif

	iDuration = htonl(iDuration);

	fseek(pFile,0,SEEK_SET);

	fseek(pFile,(iDurationByte + 2),SEEK_SET);
	fwrite(&iDuration,sizeof(int),1,pFile);

	fseek(pFile,0,SEEK_SET);

	fclose(pFile);
}

void ProcessWEBM(char* pFilePath, int* pDuration)
{
	printf("\nFinding the duration byte...(if this takes a long time, something is not right)\n");
	size_t iDurationByte = GetDurationOffset(pFilePath);

	if (GetDurationOffset <= 0)
	{
		printf("duration byte was not found. Is this .webm even valid? Quitting.");
		return;
	}

	printf("Duration byte is at: %li\n", iDurationByte);

	printf("\nChanging the duration...\n");
	AppendWEBMDuration(pFilePath, *pDuration, iDurationByte);

	printf("\nDONE!\n");
}