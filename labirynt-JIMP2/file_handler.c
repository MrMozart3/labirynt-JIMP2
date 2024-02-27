#define _CRT_SECURE_NO_WARNINGS
#include "solver.h"
#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void OpenFiles(MazeData* maze, FILE** files)
{
	char base[10] = "chunk_";
	char fileName[20];
	char num[10];
	for (int i = 0; i < maze->sizeY; i++)
	{
		strcpy(fileName, base);
		sprintf(num, "%d", i);
		strcpy(fileName, num);
		files[i] = fopen(fileName, "r+b");
	}
	return;
}

void AddNumberToString(char* output, int number)
{
	char temp[20];
	sprintf(temp, "%d", number);
	strcat(output, temp);
	return;
}

void ClearAllChunks(int max)
{
	printf("Removing Data, successfully removed:\n");
	for (int i = 0; i <= max; i++) {
		char fileName[30] = "chunk_";
		char tempNum[20];
		sprintf(tempNum, "%d", i);
		strcat(fileName, tempNum);

		if (remove(fileName) == 0)
			printf("%d ", i);
	}
	printf("\n\n");
	return;
}

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

void ReadDataInChunk(MazeData* maze, Tile* tile, int chunkIndex, int lineIndex, FILE* optionalFilePointer)
{
	int openFile = 0;

	int offset = (maze->recordSize + maze->recordOffset) * lineIndex;
	if (optionalFilePointer == NULL) {
		openFile = 1;
		char fileName[30] = "chunk_";
		AddNumberToString(fileName, chunkIndex);
		optionalFilePointer = fopen(fileName, "r+b");
	}
	if (optionalFilePointer == NULL) return;

	fseek(optionalFilePointer, offset, SEEK_SET);

	char* data = malloc(sizeof(char) * (maze->recordSize + 1));
	if (data == NULL) return;
	data[maze->recordSize] = '\0';
	fread(data, sizeof(char), maze->recordSize - 5, optionalFilePointer);
	strncpy(tile->walls, data, 4);
	tile->walls[4] = '\0';
	tile->dist = atoi(data + 5);

	if (openFile == 1) {
		fclose(optionalFilePointer);
	}

	free(data);
	return;
}

void FillChunksWithNumbers(MazeData* maze, int n)
{
	FILE* out;
	for (int y = 0; y < maze->sizeY; y++)
	{
		char fileName[30] = "chunk_";
		AddNumberToString(fileName, y);

		out = fopen(fileName, "r+b");
		if (fopen == NULL) return;

		for (int x = 0; x < maze->sizeX; x++)
		{
			EditNumberInChunk(maze, y, x, 5, n, out);
		}

		fclose(out);
	}
}

void SaveMazeToChunks(char* fileName, MazeData* maze, int fillValue)
{
	FILE* out = NULL;
	FILE* in = fopen(fileName, "r");

	int inputX = 0, inputY = 0;

	char buff[2100];
	char lines[3][2100];

	char* additionalFill = malloc(sizeof(char) * (maze->recordSize - 5 + 1));
	sprintf(additionalFill, "%d", fillValue);
	int tempLen = strlen(additionalFill);
	for (int i = tempLen; i < maze->recordSize; i++) {
		additionalFill[i] = ' ';
	}
	additionalFill[maze->recordSize] = '\0';

	int y = 0;

	while (fgets(buff, 2100, in) != NULL)
	{
		if (inputY == 0) {
			strcpy(lines[2], buff);
			maze->sizeX = (strlen(buff) - 1) / 2;
		}
		else if (inputY % 2 == 1) {
			strcpy(lines[1], buff);
		}
		else if (inputY % 2 == 0)
		{
			strcpy(lines[0], lines[2]);
			strcpy(lines[2], buff);

			//open file
			char outFileName[30] = "chunk_";
			AddNumberToString(outFileName, y);
			out = fopen(outFileName, "a+");

			//saving to file
			for (int x = 0; x < maze->sizeX; x++)
			{
				char* data = malloc(sizeof(char) * (maze->recordSize + 1));
				if (data == NULL) return;
				data[0] = lines[0][2 * x + 1];
				data[1] = lines[2][2 * x + 1];
				data[2] = lines[1][2 * x];
				data[3] = lines[1][2 * x + 2];
				data[4] = ' ';
				data[5] = '\0';
				strcat(data, additionalFill);
				data[maze->recordSize] = '\0';

				fprintf(out, "%s\n", data);

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
			y++;
			fclose(out);
		}
		inputY++;
	}
	maze->sizeY = y;
	fclose(in);
	return;
}