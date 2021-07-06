/*
====================================
Discord Video Manipulator by DrSTAHP
====================================
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#define MVHD_TIME_OFFSET 12 //12 bytes
#define BYTE 8 //8 bits
#define EIGHT_BITS 1 //1 byte

#define MAX_ARGUMENTS 255 

#define TRUE 1
#define FALSE 0

void GetOffsetData(FILE* pFile,unsigned long iOffset, void* pBuffer,size_t BufferSize)
{
    fseek(pFile, iOffset, SEEK_SET);
    fread(pBuffer, BufferSize, 1, pFile);
}


size_t GetmvhdOffset(char* pFilePath)
{
    
	FILE* pFile = NULL;
	
#ifdef _WIN32
	fopen_s(&pFile, pFilePath, "r");
#elif __linux__
	pFile = fopen(pFilePath,"r");
#endif

    size_t iOffset = 0;

    //Get the file size and reset the pointer.
    fseek(pFile,0,SEEK_END);
    size_t iFileSize = ftell(pFile);
    
    fseek(pFile,0,SEEK_SET);

    char szRequired[5] = "mvhd"; 

    while(iOffset <= iFileSize)
    {
        char szData[5];
        GetOffsetData(pFile, iOffset,szData,sizeof(szData));
     
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


//This is needed due to the fact that
//MPEG-4 headers are big-endian and modern CPUs are little-endian.
//It essentially tricks the CPU to "invert" little-endianned bytes, in effect
//creating a big-endian. 
void split_bytes(int iNumber,uint8_t* pBuffer)
{
    pBuffer[0] = (uint8_t)(iNumber >> 3*(BYTE));
    pBuffer[1] = (uint8_t)(iNumber >> 2*(BYTE));
    pBuffer[2] = (uint8_t)(iNumber >> (BYTE));
    pBuffer[3] = (uint8_t)(iNumber >> 0);
}

void AppendVideoDuration(char* pFilePath, int iDuration, size_t iMVHD)
{
	FILE* pFile = NULL;

#ifdef _WIN32
	fopen_s(&pFile,pFilePath, "r+");
#elif __linux__
    pFile = fopen(pFilePath, "r+");
#endif

    uint8_t pTimeScale[4];
    uint8_t pDuration[4];

    split_bytes(0x00000001, pTimeScale);
    split_bytes(iDuration,pDuration);

    fseek(pFile,((iMVHD + 4) + MVHD_TIME_OFFSET),SEEK_SET); //TimeScale byte.
    fwrite(pTimeScale,EIGHT_BITS,4,pFile);

    fseek(pFile,(((iMVHD + 4) + MVHD_TIME_OFFSET) + sizeof(int)),SEEK_SET); //Duration byte.
    fwrite(pDuration,EIGHT_BITS,4,pFile); 

    fclose(pFile);
}

void PromptFilePath(char* pBuffer)
{
    printf("Enter the video's path: ");
#ifdef _WIN32                           //Gonna fix those ugly preprocessor blocks later.
	scanf_s("%s", szFilePath);
#elif __linux__
	scanf("%s",pBuffer);
#endif

}

void PromptDuration(int* pBuffer)
{
    printf("Enter the video's duration: ");
#ifdef _WIN32                           //Gonna fix those ugly preprocessor blocks later.
	scanf_s("%s", szFilePath);
#elif __linux__
	scanf("%i",pBuffer);
#endif

}

void PrintHelp()
{
    printf("Flags:\n\n-f describes the file path\n-d describes the duration\n--help is used to help you\n\n");
}

void ProcessTheVideo(char* pFilePath,int* pDuration)
{
    printf("\nFinding the mvhd...(if this takes a long time, something is not right)\n");
    size_t iMVHD = GetmvhdOffset(pFilePath);

    if(iMVHD <= 0)
    {
        printf("mvhd was not found. Is this .mp4 even valid? Quitting.");
        return;
    }

    printf("mvhd is at byte: %li\n",iMVHD);

    printf("\nChanging the duration...\n");
    AppendVideoDuration(pFilePath, *pDuration, iMVHD);

    printf("\nDONE!\n");
}

void HandleArguments(int argc, char** argv)
{
    char szFileFlag[3] = "-f";
    char szDurationFlag[3] = "-d";
    char szHelp[7] = "--help";

    unsigned short iFileFlagPos = 0;
    unsigned short iDurationFlagPos = 0; 
    
    for(unsigned int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i],szFileFlag))
        {
            iFileFlagPos = i;
        }
        else if(!strcmp(argv[i],szDurationFlag))
        {
            iDurationFlagPos = i; 
        }
        else if(!strcmp(argv[i],szHelp))
        {
            PrintHelp();
            return;
        }
        else if(i != iFileFlagPos + 1 && i != iDurationFlagPos + 1) 
        {
            printf("Invalid argument %s ! Consider using --help\n\n",argv[i]);
            return;
        }
    }

    char szFilePath[2096];
    int iDuration = 1;

#ifdef _WIN32
    strcpy_s(szFilePath, argv[iFileFlagPos]);
#elif __linux__
    strcpy(szFilePath, argv[iFileFlagPos + 1]);
#endif

    iDuration = atoi(argv[iDurationFlagPos + 1]);
    
    if(!iFileFlagPos)
    {
        PromptFilePath(szFilePath);
    }

    if(!iDurationFlagPos)
    {
        PromptDuration(&iDuration);
    }

    ProcessTheVideo(szFilePath, &iDuration);
    #ifdef _WIN32
        system("pause");
    #endif
}

int main(int argc, char **argv)
{
    if(argc < 2) //First argument is the file's name in C.
    {
        //Program prompts for everything.

        char szFilePath[2096];
        int iDuration = 1;

        PromptFilePath(szFilePath);
        PromptDuration(&iDuration);
        
        ProcessTheVideo(szFilePath,&iDuration);

    #ifdef _WIN32
        system("pause");
    #endif
    }

    else if(argc >= MAX_ARGUMENTS)
    {
        printf("Exceeded the maximum argument count!\n");
    }

    else 
    {
        HandleArguments(argc,argv);
    }

    return 0;
}