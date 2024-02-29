#define _CRT_SECURE_NO_WARNINGS
#include "solver.h"
#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void AddNumberToText(char* text, int number)
{
	char tempNum[20];
	sprintf(tempNum, "%d", number);
	strcat(text, tempNum);
	return;
}

void ClearAllChunks(int max)
{
	printf("Removing Data, successfully removed:\n");
	int count = 0;
	for (int i = 0; i <= max; i++) {
		char fileName[30] = "chunk_";
		AddNumberToText(fileName, i);

		if (remove(fileName) == 0)
		count++;
	}
	printf("Removed %d files\n\n", count);
	return;
}


/*
void EditNumberInChunk(MazeData* maze, int chunkIndex, int lineIndex, int firstCharIndex, int newValue, FILE* optionalFilePointer)
{
	int openFile = 0;

	int offset = (maze->recordSize + maze->recordOffset) * lineIndex + firstCharIndex;
	if (optionalFilePointer == NULL) {
		openFile = 1;
		char fileName[30] = "chunk_";
		AddNumberToString(fileName, chunkIndex);
		optionalFilePointer = fopen(fileName, "r+b");
	}
	if (optionalFilePointer == NULL) return;

	char* data = malloc(sizeof(char) * (maze->recordSize - 5 + 1));
	if (data == NULL) return;
	data[maze->recordSize - 5] = '\0';
	sprintf(data, "%d", newValue);
	int numberLength = snprintf(NULL, 0, "%d", newValue);
	for (int i = numberLength; i < maze->recordSize - 5; i++) {
		data[i] = ' ';
	}

	fseek(optionalFilePointer, offset, SEEK_SET);
	fwrite(data, sizeof(char), strlen(data), optionalFilePointer);

	if (openFile == 1) {
		fclose(optionalFilePointer);
	}
	free(data);
	return;
}
*/

void UpdateValue(MazeData* maze, int chunkIndex, int dataIndex, int newValue)
{
	char fileName[30] = "chunk_";
	AddNumberToText(fileName, chunkIndex);
	FILE* in = fopen(fileName, "r+b");

	int offset = maze->recordSize * dataIndex + 5;
	char* newData = malloc(sizeof(char) * (maze->recordSize - 5 + 1));
	sprintf(newData, "%d", newValue);
	
	int numberLength = snprintf(NULL, 0, "%d", newValue);
	for (int i = numberLength; i < maze->recordSize - 5; i++) {
		newData[i] = ' ';
	}

	newData[maze->recordSize - 5] = '\0';

	fseek(in, offset, SEEK_SET);
	fwrite(newData, sizeof(char), maze->recordSize - 5, in);

	fclose(in);
	free(newData);
}

void LoadChunk(MazeData* maze, Tile** chunk, int chunkIndex)
{
	char fileName[30] = "chunk_";
	AddNumberToText(fileName, chunkIndex);
	FILE* in = fopen(fileName, "r+b");

	int length = maze->chunkSize * maze->chunkSize * maze->recordSize;
	char* data = malloc(sizeof(char) * (length + 1));
	fread(data, sizeof(char), length, in);
	for (int y = 0; y < maze->chunkSize; y++)
	{
		for (int x = 0; x < maze->chunkSize; x++)
		{
			for (int i = 0; i < 4; i++)
			{
				chunk[y][x].walls[i] = data[y * x + i];
			}
			chunk[y][x].walls[4] = '\0';
			char* tempNum = malloc(sizeof(char) * (maze->recordSize - 5 + 1));
			for (int i = 5; i < maze->recordSize; i++)
			{
				tempNum[i - 5] = data[y * x + i];
			}
			tempNum[maze->recordSize - 5] = '\0';
			chunk[y][x].dist = atoi(tempNum);
			free(tempNum);
		}
	}
	fclose(in);
	free(data);
}

void SaveMazeToChunks(char* fileName, MazeData* maze, int fillValue)
{
	FILE** out = NULL;
	FILE* in = fopen(fileName, "r");
	int opened = 0;

	int inputX = 0, inputY = 0;
	int horNumber;

	char buff[2100];
	char lines[3][2100];

	char* additionalFill = malloc(sizeof(char) * (maze->recordSize - 5 + 1));
	sprintf(additionalFill, "%d", fillValue);
	int tempLen = strlen(additionalFill);
	for (int i = tempLen; i < maze->recordSize - 5; i++) {
		additionalFill[i] = ' ';
	}
	additionalFill[maze->recordSize - 5] = '\0';

	char* data = malloc(sizeof(char) * (maze->recordSize + 1));

	int y = 0;
	
	char tempC;
	int countC = 0;
	while((tempC = fgetc(in)) != -1)
	{
		if(tempC != '\n'){
			buff[countC] = tempC;
			countC++;
			continue;
		}
		else{
			buff[countC] = '\0';
			countC = 0;
		}
		if (inputY == 0) {
			strcpy(lines[2], buff);
			maze->sizeX = (strlen(buff) - 1) / 2;
			horNumber = maze->sizeX % maze->chunkSize == 0 ? maze->sizeX / maze->chunkSize : maze->sizeX / maze->chunkSize + 1;
			out = malloc(sizeof(FILE*) * horNumber);
		}
		else if (inputY % 2 == 1) {
			strcpy(lines[1], buff);
		}
		else if (inputY % 2 == 0)
		{
			strcpy(lines[0], lines[2]);
			strcpy(lines[2], buff);
			//open file
			int horNumber = maze->sizeX % maze->chunkSize == 0 ? maze->sizeX / maze->chunkSize : maze->sizeX / maze->chunkSize + 1;
			if (y % maze->chunkSize == 0)
			{
				for (int i = 0; i < horNumber; i++)
				{
					char outFileName[30] = "chunk_";
					int currentChunk = (y / maze->chunkSize) * horNumber + i;
					AddNumberToText(outFileName, currentChunk);
					out[i] = fopen(outFileName, "ab");
				}
				opened = 1;
			}
			
			//saving to file
			for (int x = 0; x < maze->sizeX; x++)
			{
				
				data[0] = lines[0][2 * x + 1];
				data[1] = lines[2][2 * x + 1];
				data[2] = lines[1][2 * x];
				data[3] = lines[1][2 * x + 2];
				data[4] = ' ';
				data[5] = '\0';
				strcat(data, additionalFill);
				data[maze->recordSize] = '\0';
			
				fwrite(data, sizeof(char), maze->recordSize, out[x / maze->chunkSize]);
				
				
				//START OR END
				for (int i = 0; i < 4; i++)
				{
					if (data[i] == 'P') {
						maze->start[0] = y;
						maze->start[1] = x;
					}
					if (data[i] == 'K') {
						maze->end[0] = y;
						maze->end[1] = x;
					}
				}
			}
			if (y % maze->chunkSize == maze->chunkSize - 1)
				{
					for (int i = 0; i < horNumber; i++)
					{
						fclose(out[i]);
					}
					opened = 0;
				}
			y++;
			
		}
		inputY++;
	}
	if(opened == 1)
	{
		for (int i = 0; i < horNumber; i++)
		{
			fclose(out[i]);
		}
	}
	maze->sizeY = y;
	fclose(in);
	free(out);
	free(additionalFill);
	free(data);
	return;
}
