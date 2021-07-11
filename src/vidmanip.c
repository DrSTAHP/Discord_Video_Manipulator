/*
====================================
Discord Video Manipulator by DrSTAHP
====================================
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "mp4.h"
#include "webm.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#define FILE_FLAG (1 << 0)
#define DURATION_FLAG (1 << 1)
#define TYPE_FLAG (1 << 2)

#define MPEG4_TYPE "mp4" //Dangerous? {
#define WEBM_TYPE  "webm" //}

#define MAX_ARGUMENTS 255

#define TRUE 1
#define FALSE 0

void CopyString(const char* dst, const char* src)
{
#ifdef _WIN32
	strcpy_s(dst, sizeof(dst), src);
#elif __linux__
	strcpy(dst, src);
#endif
}

void Scan(const char* pFormat, void* pBuffer)
{
#ifdef _WIN32                          
	scanf_s(pFormat, pBuffer);
#elif __linux__
	scanf(pFormat, pBuffer);
#endif
}


void PromptFilePath(char* pBuffer)
{
	printf("Enter the video's path: ");
	Scan("%s", pBuffer);
}

void PromptDuration(int* pBuffer)
{
	printf("Enter the video's duration: ");
	Scan("%i", pBuffer);
}

void PromptType(char* pBuffer)
{
	printf("Enter the video's type (mp4/webm[NOT USABLE YET]): ");
	Scan("%s", pBuffer);
}

void PrintHelp()
{
	printf("Flags:\n\n-f describes the file path\n-d describes the duration\n-t describes the file type (mp4 ; webm[NOT USABLE YET])\n\n--help is used to help you\n\n");
}

void HandleArguments(int argc, char** argv)
{
	const char szFileFlag[3] = "-f";
	const char szDurationFlag[3] = "-d";
	const char szTypeFlag[3] = "-t";
	const char szHelp[7] = "--help";

	char szFilePath[2096];
	char szType[MAX_ARGUMENTS];
	int iDuration = 1;

	uint8_t iFlags = 0;

	for (unsigned int i = 1; i < argc; i++)
	{
		uint8_t bIsFlag = FALSE;
		
		if (i + 1 < argc) //To avoid the segmentation fault when the last flag's value is empty or not given.
		{
			if (!strcmp(argv[i], szFileFlag))
			{
				iFlags |= FILE_FLAG;
				CopyString(szFilePath, argv[i + 1]);

				bIsFlag = TRUE;
			}
			else if (!strcmp(argv[i], szDurationFlag))
			{
				iFlags |= DURATION_FLAG;
				iDuration = atoi(argv[i + 1]);

				bIsFlag = TRUE;
			}
			else if (!strcmp(argv[i], szTypeFlag))
			{
				iFlags |= TYPE_FLAG;
				CopyString(szType, argv[i + 1]);

				bIsFlag = TRUE;
			}
		}
		if (!strcmp(argv[i], szHelp))
		{
			PrintHelp();
			return;
		}
		else if (!bIsFlag && strcmp(argv[i - 1], szFileFlag) && strcmp(argv[i - 1], szDurationFlag) && strcmp(argv[i - 1], szTypeFlag))
		{
			printf("\nERROR: Invalid argument %s ! Consider using --help\n\n", argv[i]);
			return;
		}
	}

	if (!(iFlags & FILE_FLAG))
	{
		PromptFilePath(szFilePath);
	}

	if (!(iFlags & DURATION_FLAG))
	{
		PromptDuration(&iDuration);
	}

	if (!(iFlags & TYPE_FLAG))
	{
		PromptType(szType);
	}

	if (!strcmp(szType, MPEG4_TYPE))
		ProcessMP4(szFilePath, &iDuration);
	else if (!strcmp(szType, WEBM_TYPE))
		ProcessWEBM(szFilePath, &iDuration);
	else
		printf("\nERROR: Invalid video type!\n\n");

#ifdef _WIN32
	system("pause");
#endif
}

int main(int argc, char **argv)
{
	if (argc < 2) //First argument is the file's name in C.
	{
		//Program prompts for everything.

		char szFilePath[2096];
		int iDuration = 1;
		char szType[MAX_ARGUMENTS];

		PromptType(szType);
		PromptFilePath(szFilePath);
		PromptDuration(&iDuration);

		if (!strcmp(szType, MPEG4_TYPE))
			ProcessMP4(szFilePath, &iDuration);
		else if (!strcmp(szType, WEBM_TYPE))
			ProcessWEBM(szFilePath, &iDuration);
		else
			printf("\nERROR: Invalid video type!\n\n");
	}

	else if (argc >= MAX_ARGUMENTS)
	{
		printf("Exceeded the maximum argument count!\n");
	}

	else
	{
		HandleArguments(argc, argv);
	}

#ifdef _WIN32
	system("pause");
#endif
	return 0;
}