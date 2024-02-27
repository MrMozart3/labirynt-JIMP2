#define _CRT_SECURE_NO_WARNINGS
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void GetLineFromFile(char *fileName, int lineIndex, char *output)
{
	FILE* in = fopen(fileName, "r");
	char line[50];
	int lineCounter = 0;
	while (fgets(line, 50, in) != NULL)
	{
		if (lineCounter == lineIndex) {
			strcpy(output, line);
			output[strlen(output) - 1] = '\0';
			fclose(in);
			return;
		}

		lineCounter++;
	}
	fclose(in);
	return;
}

void EditLineInFile(char* fileName, int lineIndex, char* text, int addNewLine)
{
	char tempFileName[50];
	strcpy(tempFileName, fileName);
	strcat(tempFileName, "_temp");

	FILE* old_file = fopen(fileName, "r");
	FILE* new_file = fopen(tempFileName, "w");

	char line[100];
	int lineCounter = 0;
	while (fgets(line, 100, old_file) != NULL)
	{
		if (lineCounter == lineIndex) {
			fprintf(new_file, text);

			if (addNewLine == 1) {
				fprintf(new_file, "\n");
			}
		}
		else fprintf(new_file, line);

		lineCounter++;
	}
	fclose(old_file);
	fclose(new_file);
	remove(fileName);
	rename(tempFileName, fileName);
	return;
}

void ChangeOutput(FILE** stream, int chunkIndex)
{
	char fileName[30] = "chunk_";
	char tempNumber[20];
	sprintf(tempNumber, "%d", chunkIndex);
	strcat(fileName, tempNumber);

	if (*stream == NULL) {
		*stream = fopen(fileName, "a+");
	}
	else {
		fclose(*stream);
		*stream = fopen(fileName, "a+");
	}
	return;
}

int SaveMazeToChunks(char* fileName, int chunkSize, MazeData *maze, int *numberOfChunksOut)
{
	FILE* out = NULL;

	FILE* maze_input = fopen(fileName, "r");
	char buff[2100];

	int inputX = 0, inputY = 0;
	int sizeX = 0, sizeY = 0;

	char lines[3][2100];

	int y = 0;
	int currentY = 0;
	while (fgets(buff, 2100, maze_input) != NULL)
	{
		if (y == 0) {
			strcpy(lines[2], buff);
			sizeX = (strlen(buff) - 1) / 2;
		}
		else if (y % 2 == 1) {
			strcpy(lines[1], buff);
		}
		else if (y % 2 == 0)
		{
			strcpy(lines[0], lines[2]);
			strcpy(lines[2], buff);

			int currentChunk = sizeX % chunkSize == 0 ? (currentY / chunkSize) * (sizeX / chunkSize) : (currentY / chunkSize) * (sizeX / chunkSize + 1);
			int currentChunkX = 0;
			ChangeOutput(&out, currentChunk);

			
			for (int i = 0; i < sizeX; i++) {

				char fileName[30] = "chunk_";
				char tempNum[20];
				sprintf(tempNum, "%d", currentChunk);
				strcat(fileName, tempNum);

				if (i % chunkSize == 0 && currentY % chunkSize == 0) {
					if (sizeX % chunkSize != 0 && i / chunkSize == sizeX / chunkSize) {
						fprintf(out, "%d\n", sizeX - (sizeX / chunkSize) * chunkSize);
					}
					else {
						fprintf(out, "%d\n", chunkSize);
					}
				}

				char data[20] = { lines[0][2 * i + 1],lines[2][2 * i + 1],  lines[1][2 * i], lines[1][2 * i + 2] };
				strcat(data, " 100000");

				for (int j = 0; j < 4; j++)
				{
					if (data[j] == 'P') {
						maze->start[0] = (y - 1) / 2;
						maze->start[1] = i;
					}
					if (data[j] == 'K') {
						maze->end[0] = (y - 1) / 2;
						maze->end[1] = i;
					}
				}

				fprintf(out, "%s\n", data);

				currentChunkX++;
				if (currentChunkX == chunkSize) {
					currentChunk++;
					currentChunkX = 0;
					ChangeOutput(&out, currentChunk);
				}

				if (currentChunk > numberOfChunksOut[0]) {
					numberOfChunksOut[0] = currentChunk;
				}
				
			}
			currentY++;
		}
		y++;
		
	}
	sizeY = (y - 1) / 2;

	maze->sizeY = sizeY;
	maze->sizeX = sizeX;
	return 0;
}