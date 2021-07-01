/*
====================================
Discord Video Manipulator by DrSTAHP
====================================
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MVHD_TIME_OFFSET 12 //12 bytes
#define BYTE 8 //8 bits
#define EIGHT_BITS 1 //1 byte

void GetOffsetData(FILE* pFile,unsigned long iOffset, void* pBuffer,size_t BufferSize)
{
    fseek(pFile, iOffset, SEEK_SET);
    fread(pBuffer, BufferSize, 1, pFile);
}


size_t GetmvhdOffset(char* pFilePath)
{
    FILE* pFile = fopen(pFilePath,"r");

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
        
        if(!strcmp(szRequired,szData))
        {    
            fclose(pFile);
            return iOffset;
        }
        
        iOffset++;
    }
    
    fclose(pFile);
    return -1;
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
    FILE* pFile = fopen(pFilePath, "r+");

    uint8_t pTimeScale[4];
    uint8_t pDuration[4];

    split_bytes(0x00000001, pTimeScale);
    split_bytes(iDuration,pDuration);

    fseek(pFile,((iMVHD + 4) + MVHD_TIME_OFFSET),SEEK_SET);
    fwrite(pTimeScale,EIGHT_BITS,4,pFile);

    fseek(pFile,(((iMVHD + 4) + MVHD_TIME_OFFSET) + sizeof(int)),SEEK_SET);
    fwrite(pDuration,EIGHT_BITS,4,pFile); 

    fclose(pFile);
}

int main()
{
    char szFilePath[2096];
    int iDuration = 1;

    printf("Enter the video's path: ");
    scanf("%s",szFilePath);

    printf("Enter the duration in seconds: ");
    scanf("%i",&iDuration);

    printf("\nFinding the mvhd...(if this takes a long time, something is not right)\n");
    size_t iMVHD = GetmvhdOffset(szFilePath);

    if(iMVHD < 0)
    {
        printf("mvhd was not found. Is this .mp4 even valid? Quitting.");
        return 0;
    }

    printf("mvhd is at byte: %li\n",iMVHD);

    printf("\nChanging the duration...\n");
    AppendVideoDuration(szFilePath, iDuration, iMVHD);

    printf("\nDONE!\n");
    return 0;
}