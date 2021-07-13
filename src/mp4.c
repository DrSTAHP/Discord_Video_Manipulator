#include "mp4.h"
#include "shared.h"
#include <netinet/in.h>
#include <string.h>

static size_t GetmvhdOffset(char* pFilePath)
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

	const char szRequired[5] = "mvhd";

	while (iOffset <= iFileSize)
	{
		char szData[5];
		GetOffsetData(pFile, iOffset, szData, sizeof(szData));

		if (!strcmp(szRequired, szData))
		{
			fclose(pFile);
			return iOffset;
		}

		iOffset++;
	}

	fclose(pFile);
	return 0;
}

void AppendMP4Duration(char* pFilePath, int iDuration, size_t iMVHD)
{
	FILE* pFile = NULL;

#ifdef _WIN32
	fopen_s(&pFile, pFilePath, "r+");
#elif __linux__
	pFile = fopen(pFilePath, "r+");
#endif

	//htonl is used to convert the number to big endian (network byte order), regardless of host's CPU architecture. 
	const unsigned int iTimeScale = htonl(0x00000001);
	iDuration = htonl(iDuration);

	//htons is used to convert the number to big endian (network byte order), regardless of host's CPU architecture. 
	fseek(pFile, ((iMVHD + 4) + MVHD_TIME_OFFSET), SEEK_SET); //TimeScale byte.
	fwrite(&iTimeScale, sizeof(int), 1, pFile);

	fseek(pFile, (((iMVHD + 4) + MVHD_TIME_OFFSET) + sizeof(int)), SEEK_SET); //Duration byte.
	fwrite(&iDuration, sizeof(int), 1, pFile);

	fclose(pFile);
}

void ProcessMP4(char* pFilePath, int* pDuration)
{
	printf("\nFinding the mvhd...(if this takes a long time, something is not right)\n");
	size_t iMVHD = GetmvhdOffset(pFilePath);

	if (iMVHD <= 0)
	{
		printf("mvhd was not found. Is this .mp4 even valid? Quitting.");
		return;
	}

	printf("mvhd is at byte: %li\n", iMVHD);

	printf("\nChanging the duration...\n");
	AppendMP4Duration(pFilePath, *pDuration, iMVHD);

	printf("\nDONE!\n");
}
